# Plan for settings machinery prototype

Widget settings save the state of the GUI. When the user opens the application again,
it looks as it was left. This is a plan for a prototype of a unified settings machinery.
Existing code stays untouched until the pilots are proven.

## Types of settings

"Editable" means the user edits the setting in the settings dialog.
"Session" means any opened project.

| Type | Description | Examples | Current storage | Current read / write |
|---|---|---|---|---|
| (1) | Non-editable, same for all sessions | Splitter positions, tree column widths, window geometry | `QSettings` | Widget constructor / destructor |
| (2) | Editable, same for all sessions | Plugin list, undo enabled and limit | `SequencerSettingsModel` (SessionItems), flattened to `QSettings` | Main window |
| (3) | Non-editable, specific to the opened project | Panels of the splittable procedure editor and their tabs, favorite instructions | `ProjectModel` (SessionItems), in the project file | Project load / save |

## Current code

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

## Current problems

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

## Components

All new code goes to `src/lib/oac-tree-gui-components/oac_tree_gui/settings/`.
It needs QtCore only.

### Settings struct and Visit

```cpp
struct UndoInfo
{
  bool is_enabled{true};
  int undo_limit{-1};
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
- Supported field types: `bool`, `int`, `double`, `std::string`, `std::vector<std::string>`,
  nested structs, `std::vector` of structs.
- Byte arrays (splitter state) are stored as base64 strings.
- Vectors of structs are stored as `group/field/size` plus `group/field/<index>/...`.

### ISettingsStorage

```cpp
class ISettingsStorage
{
public:
  virtual ~ISettingsStorage() = default;
  virtual std::optional<mvvm::variant_t> Read(const std::string& key) const = 0;
  virtual void Write(const std::string& key, const mvvm::variant_t& value) = 0;
  virtual void RemoveGroup(const std::string& group) = 0;
};
```

Implementations:

- `InMemorySettingsStorage`. A `std::map`. Used in tests.
- `AppSettingsStorage`. For `kApplication`. Our class, backed by `QSettings`. Converts via `mvvm::GetQtVariant`.
  Reading uses the string conversion from `sup::gui::AssignStringBasedVariant`.
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

## Lifecycle

| Event | Action |
|---|---|
| Application start | `AppSettingsStorage` attached. Widgets call `Get(kApplication)` in the constructor. |
| Project loaded (`loaded_callback`) | `ProjectSettingsStorage` attached to new `ProjectModel`. `NotifyAll(kProject)`. |
| Project save / save as | `Collect(kProject)`, then save. |
| New / open / close project | `Collect(kProject)` only if the project is already modified, then proceed. |
| Application close (`CanCloseApplication`) | `Collect(kApplication)`, then the project close step above. |
| Settings dialog accepted | Dialog result converted to struct. `Set(kApplication)`. Subscribers update. |

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

### Pilot 1. Splitter (type 1)

- New `SettingsSplitter` widget next to `CustomSplitter`. Group name via constructor.
- Struct `SplitterInfo { std::string state; }`.
- Constructor: `Get(kApplication)`, restore. Registers a provider.
- Used in `SequencerComposerView` in place of `CustomSplitter`.

### Pilot 2. Undo settings (type 2)

- Struct `UndoInfo` as above.
- After the settings dialog is accepted, `SequencerSettingsModel` is converted to `UndoInfo`
  and passed to `Set(kApplication)`.
- `SequencerMainWindow` subscribes to `MainWindow/Undo` and calls `SetUndoEnabled`.
- Replaces `GetSettingsModel().Data<bool>(constants::kUseUndoSetting)` in `OnProjectLoad`.
- `SequencerSettingsModel` stays as the dialog model. It is still written to QSettings as today.

### Pilot 3. Composer panels (type 3)

- Existing `ComposerViewInfo` and `ProcedureEditorInfo` get `Visit` functions.
- `ProcedureSplittableEditorWidget` registers a provider (`GetComposerViewInfo`) and
  subscribes (`SetComposerViewInfo`) under `kProject`.
- `SetComposerViewInfo` removes existing panels before creating new ones.
- Its QSettings-based `ReadSettings` / `WriteSettings` are removed. This fixes the double
  restore in `SequencerComposerView` and the crash noted in its `WriteSettings`.
- `ComposerSettingsItem` is not used by the new path.

## Tests

New files in `test/testoac-tree-gui-components/`:

- `settings_archive_tests.cpp`. Round trip of structs through `InMemorySettingsStorage`:
  plain fields, missing keys, nested structs, vectors of structs.
- `settings_manager_tests.cpp`. Get/Set, subscriber called on `Set`, not called after
  connection is destroyed, `Collect` calls providers of one scope only, `NotifyAll`.
- `project_settings_storage_tests.cpp`. Values survive `ProjectModel` serialization.
  Writing an unchanged value does not mark the model modified.
- `settings_aware_project_manager_tests.cpp`. Collect called on save. Not called on close
  of an unmodified project. Uses a mock `IProjectManager`.

`AppSettingsStorage` is tested in `testoac-tree-gui-components-app` with a temporary
`QSettings` file.

## Steps

1. `ISettingsStorage`, `InMemorySettingsStorage`, archives, `Visit` support. Tests.
2. `SettingsManager`, `ScopedCallback`. Tests.
3. `AppSettingsStorage`, `ProjectSettingsStorage`, `SettingsContainerItem`. Tests.
4. `SettingsAwareProjectManager`. Wire manager into `SequencerMainWindowContext` and
   main window lifecycle. Tests.
5. Pilot 1.
6. Pilot 2.
7. Pilot 3.
8. Review. Decide on migration of the remaining `ReadSettings` / `WriteSettings` widgets
   and on the future of `read_variant_func_t` / `write_variant_func_t` in sup-gui-core.

## Commits

Each commit builds and passes `ctest`. Commits 1–6 only add code.

| # | Commit message | Content | Tests |
|---|---|---|---|
| 1 | Add settings storage interface and in-memory storage | `settings/` folder with CMake, `ISettingsStorage`, `InMemorySettingsStorage` | `in_memory_settings_storage_tests.cpp` |
| 2 | Add settings archives for struct serialization | `WriteArchive`, `ReadArchive`. Plain fields, nested structs, vectors of structs, base64 byte arrays | `settings_archive_tests.cpp` |
| 3 | Add settings manager | `ISettingsManager`, `SettingsManager`, `ScopedCallback`, `SettingsScope` | `scoped_callback_tests.cpp`, `settings_manager_tests.cpp` |
| 4 | Add application settings storage | `AppSettingsStorage` | Test in `testoac-tree-gui-components-app` with a temporary `QSettings` file |
| 5 | Add project settings storage | `SettingsContainerItem` in core, item registration, `ProjectSettingsStorage` | `project_settings_storage_tests.cpp`: values survive serialization, an unchanged write doesn't mark the model modified |
| 6 | Add settings-aware project manager | `SettingsAwareProjectManager` in `mainwindow/` | `settings_aware_project_manager_tests.cpp` with a mock `IProjectManager` |
| 7 | Wire settings manager into main window | `SequencerMainWindowContext` owns the manager and `AppSettingsStorage`. `SequencerMainWindowActions` uses the decorator. Attach and `NotifyAll` on project load. `Collect` in `CanCloseApplication` | Existing tests pass. Manual: save, close, open |
| 8 | Add SettingsSplitter and use it in composer view | Pilot 1 | Splitter state round trip through the in-memory storage |
| 9 | Propagate undo settings via settings manager | Pilot 2. `UndoInfo`, dialog result to `Set`, main window subscription, replaces `Data<bool>(kUseUndoSetting)` | Subscriber called after `Set`. Manual: disable undo in the dialog |
| 10 | Store composer panels in project via settings manager | Pilot 3. `Visit` for `ComposerViewInfo` and `ProcedureEditorInfo`, provider and subscriber in `ProcedureSplittableEditorWidget`, panels cleared before restore, QSettings read/write removed, commented-out code in `SequencerComposerView` removed | `ComposerViewInfo` round trip. Manual: panels restored once after reload |

- Commits 1–3 have no dependency on Qt widgets or models. They can be squashed.
- Commit 5 is the only one that touches core item registration. `ProjectModel` is unchanged.
- Commit 7 is the first behavior change. Without pilots the GUI behaves as before.
- Commits 8–10 are independent. They can be reordered or dropped.
- Step 8 has no commit. Its outcome goes into this note.

## Out of scope

- Generating settings dialog editors from `Visit`.
- Moving the machinery to sup-gui-core.
- Migration of widgets other than the pilots.
- `oac-tree-operation` and `sup-pvmonitor` applications.
