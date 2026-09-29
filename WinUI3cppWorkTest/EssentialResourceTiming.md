# WinUI Essentials: first-use resource timing

Run `WinUI3cppWorkTest` in the debugger. It opens `WETestWindow` alone so that the older `MainWindow` cannot construct `SettingsCard` or `SettingsExpander` before this test. The old prototype did not compile: `ScrollView_Loaded` returned `void` but used `co_await` (which requires a coroutine return type), and `Contact::GetContactsAsync` did not exist. This sample uses an initially empty `IObservableVector<IInspectable>` and later adds 500 `TaskSampleViewModel` objects, matching the WinRT collection interface consumed by the XAML `ItemsControl`.

## Run one case per process

For each list scenario, **restart the application** and click the named button. Cold and warm list cases exercise `ProgressBarEx`; use Direct creation to test `SettingsCard`, `SettingsExpander`, and `GroupBox` individually. Those controls also derive from `TemplateControlHelper<T>` and have a `ResourceUri`. A `SettingsExpander` resource references a `SettingsCard` resource.

| Scenario | Button | First construction | What it isolates |
| --- | --- | --- | --- |
| Direct | Direct creation | Button click, before inserting into the visual tree | Loading the dictionary without ListView layout |
| Cold list | Cold ProgressBarEx list | The already-visible ListView receives its first items; its XAML-fixed DataTemplate is unchanged | First-use dictionary insertion while XAML realizes visible task rows |
| Warm list | Prewarm ProgressBarEx, then list | Button click before items are added to the visible ListView | XAML list layout with that dictionary already inserted |

The ProgressBarEx `ListView` is visible from startup in an `Auto` Grid row with a finite viewport. Its XAML-fixed `ItemTemplate` is applied before layout, and the list binds to an empty observable collection; the test appends one item, forces a ListView measure/arrange pass, and then appends the remaining 499 task view models. This follows OpenNet’s ordering: the visible list and template exist before the page view model activates on `Loaded` and fills the collection. Do **not** wrap it in an outer `ScrollView`: unbounded measurement can realize every item and defeat the virtualization comparison. Scroll to realize later rows. The progress sample uses a local native `TaskDataRow` panel that measures and arranges 14 columns with OpenNet’s 16-pixel spacing and no fixed row height. It approximates OpenNet’s custom panel, but does not link OpenNet’s `DataRow`/`DataTable` project or use its `TaskViewModel`; the sample item binds task name, progress, colors, and visibility with typed `x:Bind` properties.

## The earlier `ItemsSource` exception

The supplied debugger trace for the previous sample ended at the projected `ListView::ItemsSource(...)` call in `RunList`, with `E_INVALIDARG`; it did not show a `Measure`/`Arrange` path or a child-collection mutation. In the warm case, the selected Essential control had already been constructed before the failing `ItemsSource` assignment. That exception therefore came from the test harness's collection hookup and did not reproduce OpenNet's reported layout exception. Changing the source to `IObservableVector<IInspectable>` with boxed strings removed that failure, which the user confirmed. The exact internal validation that rejected the earlier vector is not established by that trace, so this note does not attribute it to a particular WinUI implementation detail.

OpenNet binds an observable task collection to a `ListView` whose XAML-fixed item template contains a full-row progress layer, a status/name cell, a progress cell, and 12 other detail columns. This sample uses the same two ProgressBarEx placements, the same 14 child-to-column positions, and the same DataTable spacing. The test action adds one item, explicitly invalidates and updates the visible list to force XAML through measure/arrange, then adds the remaining tasks. The local panel exercises custom `MeasureOverride` and `ArrangeOverride`, but it still does not use OpenNet’s actual DataTable-linked panel or generated task bindings.

## Interpret results

- If direct and warm list work but cold list fails with `Child collection must not be modified during measure or arrange`, the timing of first-use dictionary insertion is implicated. This does **not** establish which WinUI element had its child collection changed: capture the first-chance native exception call stack to find that operation.
- If direct creation also fails, inspect the control constructor and dictionary load independently of ListView virtualization.
- If the ProgressBarEx cold-list case works, it shares the dictionary-loading mechanism but this combination of its template and layout did not reproduce the failure. A shared helper does not imply every control must throw.
- If the dictionary count does not rise on first construction, inspect whether another control or imported dictionary already loaded its resources before the test. Resource dictionaries may themselves merge additional dictionaries, so a delta need not equal one.

In Visual Studio, enable breaking on thrown C++/WinRT exceptions, then run the cold case in a fresh process. Record the *first* application and `Microsoft.UI.Xaml.dll` frames at the throw site, not just the later `UnhandledException` callback. Do not mark the exception handled: that would mask the crash rather than identify the child mutation.

Implementation references: `SharedComponent/ProgressBarEx.h`, `SharedComponent/SettingsCard.h`, `SharedComponent/SettingsExpander.h`, `SharedComponent/GroupBox.h`, and `WinUI3Package/include/TemplateControlHelper.hpp` in [WinUIEssentials](https://github.com/HO-COOH/WinUIEssentials).
