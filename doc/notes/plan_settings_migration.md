# Plan for settings migration

Follow-up of `plan_settings_prototype.md`. The prototype introduced the settings manager and
migrated three pilots. This plan migrates the rest of sequencer-gui, moves the machinery to
sup-gui-core, and removes the old mechanisms there.

## Goals

1. Remove `SequencerSettingsModel` from `SequencerMainWindowContext`. The model becomes a
   temporary model of the settings dialog.
2. Remove `ReadApplicationSettings` / `WriteApplicationSettings` from sequencer-gui.
3. Add `PluginSettingsInfo`, so plugin settings work as undo settings do.
4. Handle settings of all widgets in one way across sequencer-gui.
5. Move the machinery to sup-gui-core. Migrate sup-gui-core widgets and applications. Remove
   the old mechanisms there.

## Current state

### Mechanisms in use

| Mechanism | Where | Write time |
|---|---|---|
| Settings manager (`Collect`, `Get`, `Set`, `Subscribe`) | pilots: `SettingsSplitter`, `ProcedureSplittableEditorWidget`, undo settings | `Collect` in `CanCloseApplication`, project save |
| `sup::gui::CustomSplitter` | explorer, composer, operation views, `CollapsibleListView` | owner destructor |
| `sup::gui::CustomHeaderView` | most tree views | own destructor, only if the user resized columns |
| `QSettings` directly | main windows, `MessagePanel`, `MonitorRealTimeActions`, `ProcedureActionHandler`, dialogs | destructor, or on accept |
| `read_variant_func_t` / `write_variant_func_t` | `ServerSettingsWidget`, `GetDialogFileNameToOpen/Save` | destructor |
| `SequencerSettingsModel` + `Read/WriteApplicationSettings` | context constructor, `SequencerMainWindow::WriteSettings` | `CanCloseApplication` |

### sequencer-gui widgets

"Instances" is the number of instances that write the same key.

| Widget | Mechanism | Group / key | Instances |
|---|---|---|---|
| `SequencerMainWindow` | QSettings | `MainWindow/size`, `MainWindow/pos` | 1 |
| `OperationMainWindow` | QSettings | `OperationMainWindow/size`, `/pos` | 1 |
| `PvMonitorMainWindow` | QSettings | `MainWindow/size`, `/pos` | 1 |
| `SequencerExplorerView` | CustomSplitter | `SequencerExplorerView/splitter` | 1 |
| `ExplorerPanel` | CollapsibleListView | `ExplorerPanel/collapsible_list` | 1 |
| `ComposerToolsPanel` | CollapsibleListView | `ComposerPanel/collapsible_list` | 1 |
| `InstructionEditorWidget` | CustomSplitter, CustomHeaderView | `InstructionEditorWidget/splitter`, `/header_state` | one per composer panel |
| `InstructionAttributeEditor` | CustomHeaderView | `InstructionAttributeEditor/header_state` | one per composer panel |
| `WorkspaceVariableTreeView` | CustomHeaderView | `WorkspaceEditorWidget/header_state` | many: composer, operation, pvmonitor; tree and table |
| `AnyValueCompactTreeEditor` | CustomHeaderView | `AnyValueCompactTreeEditor/header_state` | one per dialog |
| `OperationMonitorView` | CustomSplitter, workdir | `OperationMonitorView/splitter`, `/workdir` | 1 |
| `OperationJobPanel` | CollapsibleListView | default group, never saved | 1 |
| `JobListWidget` | CustomHeaderView | `JobListWidget/header_state` | 1 |
| `OperationRealTimeWidget` | CollapsibleListView | `OperationRealTimeWidget/collapsible_list` | one per operation panel |
| `RealTimeInstructionTreeWidget` | CustomHeaderView | `RealTimeInstructionTreeWidget/header_state` | one per operation panel |
| `MessagePanel` | QSettings, CustomHeaderView | `MessagePanel/unchecked`, `/header_state` | one per operation panel |
| `MonitorRealTimeActions` | QSettings | `MonitorRealTimeToolBar/tick_timeout` | one per operation panel |
| `ProcedureActionHandler` | QSettings | `ProcedureActionHandler/workdir` | short-lived, many |
| `RemoteConnectionDialog` | QSettings | `RemoteConnectionDialog/size` | modal dialog |
| `ServerSettingsWidget` | read/write functions | `ServerSettingsWidget/...` (4 keys) | in the dialog |
| `PluginSettingsEditor` | DetailedMessageBox | `PluginLoadMessageBox` size | in the dialog |

### sup-gui-core widgets

| Widget | Mechanism | Group / key | Remark |
|---|---|---|---|
| `DtoEditorMainWindow` | QSettings, `PersistentSettingsModel` | `MainWindow/size`, `/position` | undo via `Data<bool>(kUseUndoSetting)` |
| `AnyValueEditorMainWindow` | QSettings, `PersistentSettingsModel` | `MainWindow/size`, `/position` | same |
| `AnyValueEditorWidget` | CustomSplitter, workdir | `AnyValueEditor/splitter`, `/workdir` | |
| `AnyValueEditorTreePanel` | CustomHeaderView | `AnyValueEditor/header_state` | |
| `WaveformEditorWidget` | CustomSplitter | `WaveformEditorWidget/splitter` | |
| `DtoCompareView`, `DtoComposerView` | CustomSplitter | `<View>/splitter` | |
| `DtoWaveformView` | CustomSplitter | `DtoWaveformView/splitter` | Read/Write bodies are empty |
| `DtoExplorerView` | CustomSplitter, ItemStackWidget | `DtoExplorerView/splitter` | Read/Write never called |
| `DtoExplorerPanel` | CollapsibleListView | `DtoExplorerPanel/collapsible_list` | |
| `CollapsibleListView` | CustomSplitter | name via constructor | owner calls Read/Write |
| `ItemStackWidget` | read/write functions | `<group>_index` | never given a group |
| `CustomHeaderView` | QSettings | name via constructor | writes only if adjusted |
| `AnyValueEditorDialog`, `DetailedMessageBox`, `SettingsEditorDialog` | QSettings | dialog size | |
| `CodeView` | QSettings | `CodeView/workdir` | |
| `GetDialogFileNameToOpen/Save` | read/write functions | caller key | last directory |
| `SetApplicationFont`, `GetAppFontFromSettings`, `SaveAppFontInSettings`, `SummonChangeSystemFontDialog` | QSettings | `MainWindow/font` | stays, see "Left on QSettings" |
| `InitCoreApplication`, `HasValidAppSettings` | QSettings | `kAppVersion` | stays |

### Other projects

No sibling project uses sup-gui-core settings APIs. `oac-tree-bundle` contains its own copies
of sequencer-gui and sup-gui-core. They are not affected until the bundle is updated.

### Problems found

- Undo settings are written twice: `MainWindow/Undo/...` by the manager and
  `SequencerSettingsModel/Undo_Settings/...` by `WriteApplicationSettings`.
- `oac-tree-operation` and `sup-pvmonitor` read `SequencerSettingsModel` from their own
  QSettings file, never write it, and have no settings dialog. Plugin settings made in
  oac-tree-gui do not reach them.
- Settings are written twice on exit via the menu: `OnExitRequest` calls
  `CanCloseApplication()`, then `close()` calls it again from `closeEvent`. The same in
  `OperationMainWindow`.
- `OperationMonitorView` writes its splitter in `closeEvent` and in the destructor.
- Multi-instance widgets share keys. The last destroyed instance wins.
- The tree and table presentations of `WorkspaceVariableTreeView` share one header key despite
  different columns.
- `ProcedureActionHandler` reads and writes QSettings on every temporary instance.
- Widget tests write the real QSettings file as a side effect of constructors and destructors.
- `ComposerSettingsItem` is still inserted into every `ProjectModel`. Not used.
- `PropagateSettingsToModel` replaces all items. Item pointers taken before it dangle.
- Dead code in sup-gui-core: `DtoWaveformView` settings, `DtoExplorerView` settings,
  `ItemStackWidget` settings, `SettingsEditorDialog::SaveToPersistentSettings()` declaration.

## Decisions

### Settings objects

- Every persisted state is a plain struct with `Visit`, as in the prototype.
- Reused structs: `SplitterInfo`, `ComposerViewInfo`, `UndoInfo`.
- New structs:

| Struct | Fields | Users |
|---|---|---|
| `PluginSettingsInfo` | `use_dir_list`, `dir_list`, `use_plugin_list`, `plugin_list` | plugin loading, settings dialog |
| `HeaderInfo` | `state` (QByteArray) | header views |
| `WindowInfo` | `geometry` (QByteArray, `saveGeometry`) | main windows, dialogs |
| `WorkdirInfo` | `path` | file dialogs, `ProcedureActionHandler`, `OperationMonitorView` |
| `MessagePanelInfo` | `unchecked` severities | `MessagePanel` |
| `RealTimeInfo` | `tick_timeout` | `MonitorRealTimeActions` |
| `ServerInfo` | server type, name, address, port | `ServerSettingsWidget` |

- `PluginSettingsInfo` defaults depend on CODAC. `CreateDefaultPluginSettingsInfo()` provides
  them. `ISettingsManager::Get` gets an initial value parameter, as `ReadFromStorage` has.

### Scope and editability

- All settings of the widget tables above go to application scope. No new settings go to
  project scope.
- None of them becomes editable in the settings dialog. The only editable settings stay undo
  settings and the plugin list.
- Moving a setting to project scope, or making it editable, is decided later, setting by
  setting. For example, it is unclear whether users want header state or window size in a
  project file.
- The composer panels of the prototype stay in both scopes.

### Groups

- Editable settings go under `Settings/`: `Settings/Undo`, `Settings/Plugins`. They no longer
  share `MainWindow` with window geometry.
- Widget groups keep today's names where possible, for example `SequencerExplorerView/splitter`.
- Old QSettings keys are not migrated. Settings files are deleted once per application.

### Two usage patterns

- **Provider.** Long-lived widgets register a provider and restore in the constructor. The
  manager collects them in `CanCloseApplication`. Splitters, headers, collapsible lists,
  panels.
- **Immediate.** Short-lived objects call `Get` on creation and `Set` when done. Dialogs,
  `ProcedureActionHandler`, file dialog helpers. No provider, as the object is gone before
  `Collect`.

### Multi-instance widgets

- Instances of one widget share one group. The state is common to all instances, as now.
- Every instance restores from the group. On `Collect`, providers are called in registration
  order, so the last registered instance wins. This replaces "last destroyed wins".
- Where a distinction is needed, the widget gets its group via constructor, as
  `ItemStackWidget` and `SettingsSplitter` do.
- The tree and table presentations of the workspace editor get different groups:
  `WorkspaceEditorWidget/tree_header`, `WorkspaceEditorWidget/table_header`.

Multi-instance widgets in sequencer-gui:

| Widget | Group today | Instances |
|---|---|---|
| `InstructionEditorWidget` | `InstructionEditorWidget/splitter`, `/header_state` | one per composer panel |
| `InstructionAttributeEditor` | `InstructionAttributeEditor/header_state` | one per composer panel |
| `WorkspaceVariableTreeView` | `WorkspaceEditorWidget/header_state` | composer panels, operation panels, pvmonitor; tree and table |
| `OperationRealTimeWidget` | `OperationRealTimeWidget/collapsible_list` | one per operation panel |
| `RealTimeInstructionTreeWidget` | `RealTimeInstructionTreeWidget/header_state` | one per operation panel |
| `MessagePanel` | `MessagePanel/unchecked`, `/header_state` | one per operation panel |
| `MonitorRealTimeActions` | `MonitorRealTimeToolBar/tick_timeout` | one per operation panel |
| `AnyValueCompactTreeEditor` | `AnyValueCompactTreeEditor/header_state` | one per open dialog |
| `ProcedureActionHandler` | `ProcedureActionHandler/workdir` | short-lived, immediate pattern |

### Manager injection

- A new struct `sup::gui::AppViewContext` bundles `IAppCommandService&` and
  `ISettingsManager&`. It lives in sup-gui-components, next to both interfaces.
- Views taking `IAppCommandService&` today take `AppViewContext&` instead, down the view
  hierarchy.
- Widgets needing only settings take `ISettingsManager&`.
- The main window context creates the `AppViewContext`.

### CustomSplitter or SettingsSplitter

`SettingsSplitter` replaces `CustomSplitter`. `CustomSplitter` is removed at the end (phase 5).

- `CustomSplitter` tracks child visibility with an event filter, because it writes in the
  destructor, when children may already be hidden. `SettingsSplitter` is collected before
  destruction, so `isHidden()` is correct and no event filter is needed.
- Both take the group via constructor. Migration is a drop-in replacement plus
  `RestoreState()` after children are added.
- `CollapsibleListView` switches its internal splitter to `SettingsSplitter`.
- `ProcedureSplittableEditorWidget` keeps its own splitter state in `ComposerViewInfo`.
- Rejected alternative: a helper registering any `QSplitter`. The caller would have to own the
  `ScopedCallback`, which `SettingsSplitter` does itself.

### Header views

- New `SettingsHeaderView` in sup-gui-views. It takes a group and `ISettingsManager&`, and
  registers a provider of `HeaderInfo`.
- It keeps the column stretch behavior of `CustomHeaderView`.
- "Write only if adjusted" is kept: the provider returns the restored state until the user
  resizes a column.
- `CustomHeaderView` is removed at the end (phase 5).

### Machinery in sup-gui-core

- `sup-gui-core` (core library) has no Qt. `sup-gui-components` has Qt via sup-mvvm view and
  viewmodel, including `mvvm::GetQtVariant` / `GetStdVariant`.

| Part | Target library |
|---|---|
| `ISettingsStorage`, `InMemorySettingsStorage`, archives, `ISettingsManager`, `SettingsManager`, `ScopedCallback` | sup-gui-components, `sup/gui/settings/` |
| `AppSettingsStorage` | sup-gui-components |
| `ProjectSettingsStorage` | sup-gui-components. Takes a function returning `mvvm::ISessionModel*` instead of `ProjectModel*` |
| `SettingsContainerItem` | sup-gui-core (core library) |
| `UndoInfo`, `UndoSettingsItem` | sup-gui-core. Replace `CommonSettingsItem` |
| `SettingsSplitter`, header view, `WindowInfo` helpers | sup-gui-views, `sup/gui/widgets/` |
| Settings model helpers pattern | sup-gui-components; items stay per application |
| `PluginSettingsInfo`, `PluginSettingsItem` | stay in sequencer-gui |

- Namespace `sup::gui`. sequencer-gui includes change from `oac_tree_gui/settings/...` to
  `sup/gui/settings/...`, and its copies are removed.
- `PersistentSettingsModel` in sup-gui-core becomes the temporary dialog model of the sup-gui-core
  applications, as `SequencerSettingsModel` does in sequencer-gui.

### Applications

- Each application owns one `SettingsManager` with `AppSettingsStorage`.
  `SequencerMainWindowContext` does it for the three sequencer-gui applications. The two
  sup-gui-core main windows get their own.
- `oac-tree-operation` and `sup-pvmonitor` read plugin settings via the manager. They keep
  their own settings files. Plugin settings are not shared between applications.
- `sup-pvmonitor` gets no project storage. Its widgets use application scope only.

### Left on QSettings

These functions of `sup/gui/mainwindow/main_window_helper.h` keep direct QSettings access:

- `SetApplicationFont()` and `GetAppFontFromSettings()`. Called from `SetupApplication()` in
  `main()`, before any main window or settings manager exists.
- `SaveAppFontInSettings()` and `SummonChangeSystemFontDialog()`. The font dialog of all
  applications. Writes the same key.
- `InitCoreApplication()` and `HasValidAppSettings()`. Store the application version, and clear
  the whole settings file if the version is too old.
- `ShouldResetSettingsAndRestart()` asks the user. The restart with
  `AppExitRequest::CleanSettingsAndRestart` clears the whole settings file.

`AppSettingsStorage` uses the same settings file, so clearing it also clears the manager's
keys.

### Closing

- `OnExitRequest` calls `close()` only. `closeEvent` calls `CanCloseApplication()` once.
- Widget destructors write nothing.

## Phases

### Phase 1. Editable settings in sequencer-gui

No sup-gui-core change. Steps A to D of `plan_settings_prototype.md` are 1.2, 1.4, 1.5 and
1.6 here. Steps 1.1, 1.3, 1.7 and 1.8 are new.

| # | Commit | Content | Tests |
|---|---|---|---|
| 1.1 | Add initial value to ISettingsManager::Get | `Get<T>(group, scope, T initial = T{})` | manager tests |
| 1.2 | Add PluginSettingsInfo | Struct, `Visit`, `CreateDefaultPluginSettingsInfo()`. `PluginSettingsItem::Get/SetPluginSettingsInfo()`. `GetPluginFileNames(const PluginSettingsInfo&)` | core item tests, visit tests |
| 1.3 | Move undo settings group | `kUndoSettingsGroup = "Settings/Undo"`, `kPluginSettingsGroup = "Settings/Plugins"` | visit tests |
| 1.4 | Add settings model helpers | `CreateSettingsModel(const ISettingsManager&)`, `WriteSettingsModel(const SequencerSettingsModel&, ISettingsManager&)` | `settings_model_helper_tests.cpp` |
| 1.5 | Use temporary settings model in dialog | `OnApplicationSettingsDialog` uses 1.4. `LoadPlugins()` reads `PluginSettingsInfo` from the manager. `SequencerSettingsModel` and `Read/WriteApplicationSettings` removed from context and main window | context test. Manual: dialog, restart, plugins |
| 1.6 | Clean up SequencerSettingsModel | `Data()` and `GetSettingsItems()` removed | model tests |
| 1.7 | Remove ComposerSettingsItem | From `ProjectModel`, registration, tests | project model tests |
| 1.8 | Close application once | `OnExitRequest` calls `close()` only, in both main windows | Manual: exit via menu and window button |

- Before 1.5, delete `~/.config/coa/oac-tree-gui.conf`.

### Phase 2. Machinery to sup-gui-core

sup-gui-core gets the machinery next to the old mechanisms. Nothing is removed yet.

| # | Repo | Commit | Content |
|---|---|---|---|
| 2.1 | sup-gui-core | Add settings machinery | Storage interface, in-memory storage, archives, manager, `ScopedCallback`, `AppSettingsStorage`, with tests |
| 2.2 | sup-gui-core | Add SettingsContainerItem and ProjectSettingsStorage | Generic model function |
| 2.3 | sup-gui-core | Add UndoInfo and UndoSettingsItem | Next to `CommonSettingsItem` |
| 2.4 | sup-gui-core | Add SettingsSplitter | In `sup/gui/widgets/` |
| 2.5 | sup-gui-core | Add AppViewContext | `IAppCommandService&` and `ISettingsManager&` |
| 2.6 | sequencer-gui | Use settings machinery from sup-gui-core | Includes changed, own copies and tests removed |

- 2.6 needs the sup-gui-core PR.

### Phase 3. sup-gui-core widgets and applications

| # | Commit | Content |
|---|---|---|
| 3.1 | Settings manager in sup-gui-core main windows | `DtoEditorMainWindow`, `AnyValueEditorMainWindow` own a manager. Geometry via `WindowInfo` |
| 3.2 | Undo settings via manager | `UndoSettingsItem` replaces `CommonSettingsItem`. `PersistentSettingsModel` becomes the temporary dialog model. `Read/WriteApplicationSettings` calls removed |
| 3.3 | Add SettingsHeaderView | `HeaderInfo`, provider, "write only if adjusted" kept. Users of `CustomHeaderView` in sup-gui-core switch to it |
| 3.4 | CollapsibleListView with settings manager | Internal `SettingsSplitter` |
| 3.5 | Migrate anyvalue editor widgets | `AnyValueEditorWidget`, tree panel, dialog size, workdir |
| 3.6 | Migrate dto editor views | `DtoCompareView`, `DtoComposerView`, `DtoExplorerPanel`, `WaveformEditorWidget`. Dead settings code removed |
| 3.7 | Workdir and dialog size helpers | `GetDialogFileNameToOpen/Save` take `ISettingsManager&` and a group. `DetailedMessageBox`, `SettingsEditorDialog`, `CodeView` |

- Each step keeps the old API until phase 5, if sequencer-gui still uses it.

### Phase 4. sequencer-gui widgets

One commit per view. Order follows the dependencies on phase 3.

| # | Commit | Content |
|---|---|---|
| 4.1 | Use AppViewContext in views | Views taking `IAppCommandService&` take `AppViewContext&`. No settings change |
| 4.2 | Main window geometry via manager | Three main windows, `WindowInfo` |
| 4.3 | Migrate explorer view | `SequencerExplorerView` splitter, `ExplorerPanel` list |
| 4.4 | Migrate composer widgets | `ComposerToolsPanel`, `InstructionEditorWidget`, `InstructionAttributeEditor`, workspace headers. `SettingsSplitter`, `SettingsHeaderView` |
| 4.5 | Migrate operation view | `OperationMonitorView`, `JobListWidget`, `OperationJobPanel` |
| 4.6 | Migrate operation panel widgets | `OperationRealTimeWidget`, `RealTimeInstructionTreeWidget`, `MessagePanel`, `MonitorRealTimeActions` |
| 4.7 | Migrate short-lived objects | `ProcedureActionHandler`, `RemoteConnectionDialog`, `ServerSettingsWidget`, `AnyValueCompactTreeEditor` |
| 4.8 | Migrate pvmonitor | `MonitorWidget` headers |

- Widget tests use `InMemorySettingsStorage`. They stop writing the real settings file.

### Phase 5. Cleanup in sup-gui-core

| # | Commit | Content |
|---|---|---|
| 5.1 | Remove CustomSplitter | Class, controller, tests |
| 5.2 | Remove CustomHeaderView | Class, tests |
| 5.3 | Remove settings callbacks | `settings_callbacks.h`, `read/write_variant_func_t` |
| 5.4 | Remove settings helper | `Read/WriteApplicationSettings`, tests |
| 5.5 | Remove CommonSettingsItem | Class, registration, tests |
| 5.6 | Remove dead declarations | `SettingsEditorDialog::SaveToPersistentSettings()` |

## Open questions

None.

## Out of scope

- Generating settings dialog editors from `Visit`.
- New project-scope settings. New editable settings.
- Sharing plugin settings between applications.
- Migration of old QSettings keys.
- Recent projects and recent directories kept by sup-mvvm (`ProjectHandler`,
  `RecentProjectSettings`, `FileTreeView`).
- Application font, version check and settings reset. They stay on QSettings.
- `oac-tree-bundle` copies.
