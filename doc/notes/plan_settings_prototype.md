# Plan for settings machinery prototype

Widget settings save the state of the GUI. When the user opens the application again,
it looks as it was left. This is a plan for a prototype of a unified settings machinery.
Existing code stays untouched until the pilots are proven.

Sections "Types of settings" to "Requirements" describe the code before the prototype.

## Types of settings

"Editable" means the user edits the setting in the settings dialog.
"Session" means any opened project.

| Type | Description | Examples | Current storage | Current read / write |
|---|---|---|---|---|
| (1) | Non-editable, same for all sessions | Splitter positions, tree column widths, window geometry | `QSettings` | Widget constructor / destructor |
| (2) | Editable, same for all sessions | Plugin list, undo enabled and limit | `SequencerSettingsModel` (SessionItems), flattened to `QSettings` | Main window |
| (3) | Non-editable, specific to the opened project | Panels of the splittable procedure editor and their tabs, favorite instructions | `ProjectModel` (SessionItems), in the project file | Project load / save |

## Code before the prototype

- Type 1.
  - `sup::gui::CustomSplitter`, `ProcedureSplittableEditorWidget::ReadSettings/WriteSettings`,
    `*MainWindow::ReadSettings/WriteSettings`, `ProcedureActionHandler`.
  - Access via `sup::gui::read_variant_func_t` / `write_variant_func_t`
    (`sup/gui/widgets/settings_callbacks.h`), or `QSettings` directly.
- Type 2.
  - `SequencerSettingsModel` holds `sup::gui::CommonSettingsItem` and `PluginSettingsItem`.
  - Owned by `SequencerMainWindowContext`. Read from `QSettings` in its constructor with
    `sup::gui::ReadApplicationSettings`.
  - Written in `SequencerMainWindow::WriteSettings` with `sup::gui::WriteApplicationSettings`.
    It walks the item tree and writes `ModelType/Item/Property` keys.
  - Edited in `SequencerMainWindowActions::OnApplicationSettingsDialog` via
    `sup::gui::SettingsEditorDialog`. `PluginSettingsItem` has a custom editor.
  - Consumed as `GetSettingsModel().Data<bool>(constants::kUseUndoSetting)` in
    `SequencerMainWindow::OnProjectLoad`.
- Type 3.
  - `ComposerSettingsItem` in `ProjectModel`. Converts to and from `ComposerViewInfo`.
  - Read in `SequencerComposerView::SetModels`. Writing is commented out.

## Problems before the prototype

- Two storages: `QSettings` for simple values, SessionModel for complex ones. Type 2
  SessionModel goes to `QSettings`, type 3 SessionModel goes to the project file.
- No common settings object. Light-weight structs (`ComposerViewInfo`) are mixed with direct
  property access on SessionItems.
- A widget is not told when its settings change from outside.
- A widget cannot be asked for its current state, for example before the application closes.
- Writing in the destructor is too late. The widget cannot report its visibility.
  Models may already be destroyed: `SequencerComposerView::WriteSettings` crashes on
  `ProjectModel` access.
- `SequencerComposerView` restores panels twice. The constructor creates panels from
  `QSettings`, then `SetModels` creates more from `ComposerSettingsItem`. This repeats on
  every project load.
- `ProcedureSplittableEditorWidget` duplicates the same logic in `ReadSettings/WriteSettings`
  and `GetComposerViewInfo/SetComposerViewInfo`.
- `mvvm::ProjectContext` has no "about to save" callback. Only `modified_callback`,
  `loaded_callback` and `saved_callback`.

## Requirements

- A settings manager is injected into every widget that has settings.
- A widget can read the current value of its settings.
- A widget can write its settings at any moment.
- The manager can ask a widget for its current settings, for example before closing.
- A widget can subscribe to changes of its settings made elsewhere.
- Each setting is stored in one of two storages: persistent (all sessions) or project file.

Target usage:

```cpp
UndoInfo undo_info = manager.Get<UndoInfo>("MainWindow", SettingsScope::kApplication);
manager.Set("MainWindow", undo_info, SettingsScope::kApplication);
```

## Decisions

- A settings object is a plain struct. It is not a `SessionItem`.
- Each struct has one `Visit` function. It serves both read and write.
- Storage is a flat key/value backend. Keys are `Group/Key` strings. Values are `mvvm::variant_t`.
- Two scopes: `kApplication` (QSettings) and `kProject` (project file).
- Widgets do not write settings in the destructor. The main window asks them to provide
  their state (`Collect`).
- Project-scope state is collected only at save time. Changing the layout does not mark
  the project as modified. Layout of an unmodified project is lost on close. This is accepted.
- Widgets with several instances get their settings group name via constructor, as
  `sup::gui::CustomSplitter` does.
- `Collect(kProject)` is called explicitly in sequencer-gui. No new hook in sup-mvvm.
- The settings manager is the source of truth for editable settings too.
  `SequencerSettingsModel` is only a temporary model for `SettingsEditorDialog`.
- sup-gui-core is not changed during the prototype. Generic parts move there afterwards.
- Old `QSettings` keys are not migrated. `~/.config/coa/oac-tree-gui.conf` is deleted once.

## Components

The machinery goes to `src/lib/oac-tree-gui-components/oac_tree_gui/settings/`.
It needs QtCore only. Settings structs of editable settings live in core, next to their
items, as core cannot depend on components.

### Settings struct and Visit

```cpp
struct UndoInfo
{
  bool is_enabled{constants::kUseUndoDefault};
  mvvm::int32 undo_limit{constants::kUndoLimitDefault};
};

template <typename Archive>
void Visit(Archive& archive, UndoInfo& info)
{
  archive("is_enabled", info.is_enabled);
  archive("undo_limit", info.undo_limit);
}
```

- `WriteArchive` puts every field into a storage under `group/field`.
- `ReadArchive` reads every field. A missing key keeps the default value of the struct.
- Supported field types: `bool`, `int32`, `double`, `std::string`, `QByteArray`, enums,
  nested structs, `std::vector` of any of them except `bool`.
- Byte arrays (splitter state) are stored as base64 strings.
- Vectors of structs are stored as `group/field/size` plus `group/field/<index>/...`.
- Shrinking a vector leaves stale elements in the storage. They are not read.

### ISettingsStorage

```cpp
class ISettingsStorage
{
public:
  virtual ~ISettingsStorage() = default;
  virtual std::optional<mvvm::variant_t> Read(const std::string& key) const = 0;
  virtual void Write(const std::string& key, const mvvm::variant_t& value) = 0;
};
```

Implementations:

- `InMemorySettingsStorage`. A `std::map`. Used in tests.
- `AppSettingsStorage`. For `kApplication`. Our class. Works via injected read and write
  functions. `CreateAppSettingsStorage()` wires them to `QSettings`. Converts via
  `mvvm::GetQtVariant` / `mvvm::GetStdVariant`. QSettings returns strings after restart, so
  `FromSettingsValue` parses strings for `bool`, `int32` and `double`.
- `ProjectSettingsStorage`. For `kProject`. Keeps values in a new `SettingsContainerItem`
  inside `ProjectModel`. One property per key. The item is created lazily, so `ProjectModel`
  and the project file format stay unchanged for the prototype. The storage is re-attached on
  every project load.

### ISettingsManager

```cpp
enum class SettingsScope : std::uint8_t { kApplication, kProject };

class ISettingsManager
{
public:
  // untyped core
  virtual ISettingsStorage* GetStorage(SettingsScope scope) const = 0;
  virtual ScopedCallback AddSubscriber(const std::string& group, SettingsScope scope,
                                           settings_reader_t reader) = 0;
  virtual ScopedCallback AddProvider(const std::string& group, SettingsScope scope,
                                         settings_writer_t writer) = 0;
  virtual void NotifyGroup(const std::string& group, SettingsScope scope) = 0;
  virtual void Collect(SettingsScope scope) = 0;
  virtual void NotifyAll(SettingsScope scope) = 0;

  // typed wrappers
  template <typename T> T Get(const std::string& group, SettingsScope scope) const;
  template <typename T> void Set(const std::string& group, const T& value, SettingsScope scope);

  template <typename T>
  ScopedCallback Subscribe(const std::string& group, SettingsScope scope,
                               std::function<void(const T&)> callback);

  template <typename Func>
  ScopedCallback RegisterProvider(const std::string& group, SettingsScope scope,
                                      Func provider);
};
```

- `Get`: reads the struct from the storage of the given scope.
- `Set`: writes the struct and calls the subscribers of the group, including the caller's own
  subscriber.
- `Subscribe`: the widget is told when the group changes. `T` is given explicitly.
- `RegisterProvider`: the widget tells the manager how to obtain its current state. The struct
  type is deduced from the provider's return type.
- `Collect`: calls every provider of the scope and writes the results. Does not notify.
- `NotifyAll`: calls every subscriber of the scope with the stored value. Used after
  project load.
- Without storage for the scope, `Get` and `Set` throw `RuntimeException`. `Collect`,
  `NotifyAll` and `NotifyGroup` do nothing. Widgets receive project settings via subscription,
  not via `Get`.
- Subscribers and providers are called in registration order. A subscriber removed during a
  notification is not called. A subscriber added during a notification waits for the next one.
- `Subscribe` and `RegisterProvider` return a `ScopedCallback`. It is a generic move-only
  class calling a callback on destruction. The manager puts there a callback removing the
  subscriber or provider. The widget owns it as a member. The manager must outlive it.
- Templates are thin wrappers over the virtual untyped core, so the manager can be mocked.
- Class `SettingsManager` implements it. It owns no storage. Storages are set with
  `SetStorage(scope, ISettingsStorage*)`.

### Injection

- `SequencerMainWindowContext` owns `SettingsManager` and `AppSettingsStorage`.
- Views receive `ISettingsManager&` via constructor, next to `IAppCommandService&`.
- For the prototype, only the pilot widgets get the new parameter.

### Editable settings

Each item of `SequencerSettingsModel` has a settings struct behind it.

| Item | Struct | Group |
|---|---|---|
| `UndoSettingsItem` | `UndoInfo` | `MainWindow/Undo` |
| `PluginSettingsItem` | `PluginSettingsInfo` | `MainWindow/Plugins` |

- Structs, their `Visit` and group names are in core, next to the items.
- Items provide `Get...Info()` / `Set...Info()`.
- `UndoSettingsItem` replaces `sup::gui::CommonSettingsItem`, so sup-gui-core stays untouched.
- Two helpers in `components/settings/settings_model_helper.h` connect the model and the
  manager:

```cpp
//! Creates settings model populated from the settings manager.
std::unique_ptr<SequencerSettingsModel> CreateSettingsModel(const ISettingsManager& manager);

//! Writes settings from the model to the settings manager.
void WriteSettingsModel(const SequencerSettingsModel& model, ISettingsManager& manager);
```

- They are the only place that maps items to structs and groups.
- The dialog flow:

```cpp
auto model = CreateSettingsModel(settings_manager);
sup::gui::SettingsEditorDialog dialog(*model);
...
if (dialog.exec() == QDialog::Accepted)
{
  dialog.PropagateSettingsToModel(*model);  // replaces all items of the model
  WriteSettingsModel(*model, settings_manager);
}
```

- `PropagateSettingsToModel` replaces the root item. Pointers to items taken before it are
  invalid.
- `SequencerMainWindowContext` does not own `SequencerSettingsModel`. `LoadPlugins()` reads
  `PluginSettingsInfo` from the manager.
- `ReadApplicationSettings` / `WriteApplicationSettings` from sup-gui-core are not used.

## Lifecycle

| Event | Action |
|---|---|
| Application start | `AppSettingsStorage` attached. `LoadPlugins()` reads `PluginSettingsInfo`. Widgets call `Get(kApplication)` in the constructor. |
| Project loaded (`loaded_callback`) | `ProjectSettingsStorage` attached to new `ProjectModel`. `NotifyAll(kProject)`. |
| Project save / save as | `Collect(kProject)`, then save. |
| New / open / close project | `Collect(kProject)` only if the project is already modified, then proceed. |
| Application close (`CanCloseApplication`) | `Collect(kApplication)`, then the project close step above. |
| Settings dialog opened | Temporary `SequencerSettingsModel` created by `CreateSettingsModel`. |
| Settings dialog accepted | `WriteSettingsModel` calls `Set(kApplication)` for each struct. Subscribers update. |

### Explicit project collect

The `mvvm::Add*ProjectAction` helpers take an `mvvm::IProjectManager&`. Add a decorator
`SettingsAwareProjectManager : mvvm::IProjectManager` in `mainwindow/`. It wraps
`mvvm::ProjectHandler` and calls the manager before forwarding:

- `SaveCurrentProject`, `SaveProjectAs`: always `Collect(kProject)`.
- `CreateNewProject`, `OpenExistingProject`, `CloseProject`: `Collect(kProject)` only if
  `GetProject()->IsModified()`. Otherwise collecting would mark the project modified and
  trigger the "save changes?" question.

`SequencerMainWindowActions` passes the decorator instead of the handler to the helpers.

## Pilots

One pilot per settings type.

### Pilot 1. Splitter (type 1). Done

- `SettingsSplitter` in `views/widgets/`. Group name via constructor.
- Struct `SplitterInfo { QByteArray state; std::vector<std::int32_t> visibility; }`.
- Registers a provider in the constructor. `RestoreState()` is called after the children are
  added.
- Child visibility is read with `isHidden()` on `Collect`. No event filter as in
  `CustomSplitter`.
- Used in `SequencerComposerView` in place of `CustomSplitter`.

### Pilot 2. Undo settings (type 2). Done

- `UndoInfo` and `UndoSettingsItem` in core.
- `SequencerMainWindow` subscribes to `MainWindow/Undo` and calls `SetUndoEnabled`. On project
  load it uses `Get<UndoInfo>`.
- `OnApplicationSettingsDialog` fills `UndoSettingsItem` from the manager and calls `Set` on
  accept. This moves to the helpers of the "Editable settings" section.

### Pilot 3. Composer panels

Done in two stages: application scope first (D1), project scope later (E3).

#### Stage 1. Application scope (D1)

The user gets the same number of split panels, and the same tabs in them, even without saving
the project.

- Existing `ComposerViewInfo` and `ProcedureEditorInfo` in core get `Visit` functions. One
  struct holds splitter state, and editor type and procedure id of each panel.
- `ProcedureEditorInfo::editor_type` gets the default `kInstructionTree`. A missing key keeps
  the default.
- `splitter_state` stays a base64 `std::string`, as in `ComposerSettingsItem`.
- `ProcedureSplittableEditorWidget` gets `ISettingsManager&` via constructor. It registers a
  provider (`GetComposerViewInfo`) under `kApplication`.
- The widget has one instance. Its group is a constant in its header,
  `kComposerPanelsSettingsGroup = "ProcedureSplittableEditorWidget"`, as `kUndoSettingsGroup`.
  It moves to the constructor if a second instance appears.
- Panels are restored once, in the constructor, from `Get<ComposerViewInfo>`. No stored panels
  means one panel. Rebuilding panels on every project load would discard the layout changed
  during the session, as the stored value is refreshed only on application close.
- `SetComposerViewInfo` is the only restore path. Stage 2 reuses it for project scope. It
  removes existing panels, creates new ones, assigns procedures if the model is set, and
  restores the splitter state.
- Existing panels are deleted immediately after `ClosePanel`, as `ClosePanel` uses
  `deleteLater` and the hidden panel stays in the splitter. Not needed in the constructor,
  needed in stage 2.
- Procedure ids are stored too, for uniformity with project scope.
- Procedures are assigned on `SetModel`, to every panel. `SplittableComboPanel::SetModel`
  resets the selection, and `ComposerToolsPanel` selects a procedure in the focus panel only.
  A stored procedure id selects the procedure if it exists in the model, otherwise the first
  procedure is used (`FindAppropriateProcedure`).
- A freshly started application has one untitled procedure. Stored ids are not found, so all
  restored panels show this procedure. Procedures that existed on close are ignored.
- `CanCloseApplication` calls `Collect(kApplication)` before `CloseCurrentProject()`.
  Closing the project destroys the models, and the provider reads procedure ids.
- Removed:
  - QSettings-based `ReadSettings` / `WriteSettings` and their keys (`panel_count`,
    `splitter`, `procedure_tabs`). Only `GetComposerViewInfo` / `SetComposerViewInfo` remain.
  - Panel creation from `ComposerSettingsItem` in `SequencerComposerView::SetModels`.
  - `SequencerComposerView::WriteSettings` and the destructor calling it.
- This fixes the double restore in `SequencerComposerView`.

#### Stage 2. Project scope (E3)

- The same `ComposerViewInfo` is also stored in `kProject`, so each project keeps its own
  panels and procedures. Application scope stays.
- `ProcedureSplittableEditorWidget` adds, under the same group constant:
  - a provider in `kProject` returning `GetComposerViewInfo`;
  - a subscriber in `kProject` calling `SetComposerViewInfo`.
- The widget keeps four `ScopedCallback` members: provider and subscriber per scope. The
  application-scope subscriber is not needed, as nobody calls `Set` for this group.
- Subscribing before the project storage is attached is allowed. `NotifyAll` does nothing
  without storage.
- On project load, `NotifyAll(kProject)` is called after `SequencerComposerView::SetModels`.
  Procedure ids are resolved against the new model. The project value also overrides the
  procedure that `ComposerToolsPanel` selects in the focus panel.
- A project without stored panels gives an empty `editor_info_list`. The subscriber ignores
  it and keeps the current panels, so the application scope layout is the default. A stored
  empty list cannot be told apart from a missing one. This is accepted.
- `ComposerSettingsItem` is not used by the new path.

## Tests

Existing, in `test/testoac-tree-gui-components/` unless noted:

- `in_memory_settings_storage_tests.cpp`, `settings_archive_tests.cpp`.
- `scoped_callback_tests.cpp`, `settings_manager_tests.cpp`.
- `app_settings_storage_tests.cpp`. Mock functions and a fake emulating an INI file after
  restart. No files are created.
- `settings_splitter_tests.cpp` in `testoac-tree-gui-views`.
- `undo_settings_item_tests.cpp`, `sequencer_settings_model_tests.cpp` in
  `testoac-tree-gui-core`.

Planned:

- Plugin settings item tests for `PluginSettingsInfo` get/set, in core.
- `settings_model_helper_tests.cpp`. Model populated from and written to the manager with
  `InMemorySettingsStorage`.
- `project_settings_storage_tests.cpp`. Values survive `ProjectModel` serialization.
  Writing an unchanged value does not mark the model modified.
- `settings_aware_project_manager_tests.cpp`. Collect called on save. Not called on close
  of an unmodified project. Uses a mock `IProjectManager`.
- `ComposerViewInfo` round trip through the archive, in components. Core tests cannot use
  the archive.
- `procedure_splittable_editor_widget_tests.cpp` in `testoac-tree-gui-views` uses
  `SettingsManager` with `InMemorySettingsStorage`. Tests of `ReadSettings` / `WriteSettings`
  removed. New tests: one panel by default, panels and tabs restored, `Collect` round trip,
  procedures assigned on `SetModel`, `SetComposerViewInfo` replaces panels.

## Status

Done:

| Part | Commits |
|---|---|
| Storage interface, in-memory storage | `26af478d` |
| Archives, string parsing for QSettings | `86447316`, `b468135d`, `e8c8983d`, `b1f6b13b` |
| `ScopedCallback`, settings manager | `b214904e`, `ae5f60d6` |
| `AppSettingsStorage`, `RemoveGroup` removed | `eeb1957f`, `407e8b97` |
| Manager in main windows, application scope | `61630a3c` |
| Pilot 1: `SettingsSplitter` in composer view | `c6dbbe97`, `9e82987e` |
| Pilot 2: `UndoSettingsItem`, undo via manager | `ccf27afb`, `e250bffd` |

## Next steps

| # | Commit | Content | Tests |
|---|---|---|---|
| A | Add PluginSettingsInfo | Struct and `Visit` in core. `PluginSettingsItem::Get/SetPluginSettingsInfo()`. `GetPluginFileNames(const PluginSettingsInfo&)` | Core tests for item and struct |
| B | Add settings model helpers | `CreateSettingsModel`, `WriteSettingsModel` in `components/settings/` | `settings_model_helper_tests.cpp` |
| C | Use temporary settings model in dialog | `OnApplicationSettingsDialog` uses B. `LoadPlugins()` reads from the manager. `SequencerSettingsModel` removed from `SequencerMainWindowContext`. `ReadApplicationSettings` / `WriteApplicationSettings` calls removed | Context test updated. Manual: dialog, restart |
| D | Clean up SequencerSettingsModel | `Data()` and `GetSettingsItems()` removed. `Get()` via `GetTopItem` | Model tests updated |
| D1 | Clean up ProcedureSplittableEditorWidget | Pilot 3, stage 1. `Visit` for `ComposerViewInfo` / `ProcedureEditorInfo`. Panels stored in application scope under a constant group. Duplicated QSettings read/write and panel creation in `SetModels` removed. `Collect(kApplication)` moved before project close | `ComposerViewInfo` round trip. Widget test: panels and tabs restored. Manual: split panels, change tabs, restart without saving |
| E1 | Add project settings storage | `SettingsContainerItem` in core, `ProjectSettingsStorage` | `project_settings_storage_tests.cpp` |
| E2 | Add settings-aware project manager | `SettingsAwareProjectManager`. Attach project storage and `NotifyAll(kProject)` on project load | `settings_aware_project_manager_tests.cpp` |
| E3 | Store composer panels in project | Pilot 3, stage 2. `ProcedureSplittableEditorWidget` adds provider and subscriber in `kProject` under the same group. `NotifyAll(kProject)` after `SetModels`. Empty list keeps current panels | Widget test: project notification replaces panels and procedures, empty list ignored. Manual: panels and procedures restored per project |

- A and B only add code. C and D1 change behavior.
- Before C, delete `~/.config/coa/oac-tree-gui.conf`.
- Project scope (E1–E3) comes after D1.

After the prototype:

- Review. Decide on migration of the remaining `ReadSettings` / `WriteSettings` widgets.
- Move the machinery, `UndoInfo` and `UndoSettingsItem` to sup-gui-core. Replace
  `CommonSettingsItem` and `PersistentSettingsModel` there. First users:
  `DtoEditorMainWindow`, `AnyValueEditorMainWindow`.
- Remove the unused declaration `SettingsEditorDialog::SaveToPersistentSettings()` in
  sup-gui-core.
- Consider a helper registering any `QSplitter` instead of the `SettingsSplitter` subclass.

## Out of scope

- Generating settings dialog editors from `Visit`.
- Migration of old `QSettings` keys.
- Live update of the enabled state of undo/redo actions after the settings dialog.
- Migration of widgets other than the pilots.
- `oac-tree-operation` and `sup-pvmonitor` applications.
