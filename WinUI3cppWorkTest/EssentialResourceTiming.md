# WinUI Essentials: first-use resource timing

Run `WinUI3cppWorkTest` in the debugger. It opens `WETestWindow` alone so that the older `MainWindow` cannot construct `SettingsCard` or `SettingsExpander` before this test. The old prototype did not compile: `ScrollView_Loaded` returned `void` but used `co_await` (which requires a coroutine return type), and `Contact::GetContactsAsync` did not exist. This sample uses an `IObservableVector<IInspectable>` with 500 boxed string items, matching the WinRT collection interface consumed by the XAML `ItemsControl`.

## Run one case per process

For each row below, **restart the application**, select exactly one control and click the named button. Repeat with each control: `ProgressBarEx`, `SettingsCard`, `SettingsExpander`, `GroupBox`. The controls each derive from `TemplateControlHelper<T>` and have a `ResourceUri`. The first instance of each type may append its own resource dictionary to the *application-wide* `MergedDictionaries` collection. A `SettingsExpander` resource also references a `SettingsCard` resource; therefore testing them successively in one process would make the results ambiguous.

| Scenario | Button | First construction | What it isolates |
| --- | --- | --- | --- |
| Direct | Direct creation | Button click, before inserting into the visual tree | Loading the dictionary without ListView layout |
| Cold list | Cold virtualized list | XAML measures a populated, initially collapsed ListView after its DataTemplate is installed | First-use dictionary insertion while XAML realizes visible task rows |
| Warm list | Prewarm, then list | Button click before the ListView is made visible | XAML list layout with that dictionary already inserted |

The populated `ListView` is `Collapsed` at startup and has a finite viewport in an `Auto` Grid row. The test assigns its `ItemTemplate`, then changes `Visibility` to `Visible` and invalidates measure. That asks the XAML layout system to measure the list and realize its visible template rows; the test does not synchronously construct rows in the button handler. Do **not** wrap it in an outer `ScrollView`: unbounded measurement can realize every item and defeat the virtualized-row comparison. The 500 items make it easy to verify that only a small portion initially loads; scroll to realize subsequent rows. The window logs dictionary count before and after first construction and when the first three rows raise `Loaded`. `Loaded` is after layout has created the row; it does not by itself prove exactly which internal WinUI method constructed the control. The row layout uses Grid columns to approximate OpenNet’s `DataRow`; this project does not reference OpenNet’s custom `DataRow`/`DataTable` toolkit.

## The earlier `ItemsSource` exception

The supplied debugger trace for the previous sample ended at the projected `ListView::ItemsSource(...)` call in `RunList`, with `E_INVALIDARG`; it did not show a `Measure`/`Arrange` path or a child-collection mutation. In the warm case, the selected Essential control had already been constructed before the failing `ItemsSource` assignment. That exception therefore came from the test harness's collection hookup and did not reproduce OpenNet's reported layout exception. Changing the source to `IObservableVector<IInspectable>` with boxed strings removed that failure, which the user confirmed. The exact internal validation that rejected the earlier vector is not established by that trace, so this note does not attribute it to a particular WinUI implementation detail.

OpenNet binds an observable task collection to a `ListView` whose header and task rows use multiple aligned columns, status/name stacks, a progress cell, and a details region below. This sample now mirrors that overall geometry and starts with the real row collection already bound through `x:Bind`; the test action only installs the selected template and reveals the list so XAML must lay it out. It still approximates OpenNet's custom `DataRow` with a `Grid`, so a remaining difference in the toolkit panel's measure behavior can matter.

## Interpret results

- If direct and warm list work but cold list fails with `Child collection must not be modified during measure or arrange`, the timing of first-use dictionary insertion is implicated. This does **not** establish which WinUI element had its child collection changed: capture the first-chance native exception call stack to find that operation.
- If direct creation also fails, inspect the control constructor and dictionary load independently of ListView virtualization.
- If a control works cold, it shares the dictionary-loading mechanism but this combination of control template and layout did not reproduce the failure. A shared helper does not imply every control must throw.
- If the dictionary count does not rise on first construction, inspect whether another control or imported dictionary already loaded its resources before the test. Resource dictionaries may themselves merge additional dictionaries, so a delta need not equal one.

In Visual Studio, enable breaking on thrown C++/WinRT exceptions, then run the cold case in a fresh process. Record the *first* application and `Microsoft.UI.Xaml.dll` frames at the throw site, not just the later `UnhandledException` callback. Do not mark the exception handled: that would mask the crash rather than identify the child mutation.

Implementation references: `SharedComponent/ProgressBarEx.h`, `SharedComponent/SettingsCard.h`, `SharedComponent/SettingsExpander.h`, `SharedComponent/GroupBox.h`, and `WinUI3Package/include/TemplateControlHelper.hpp` in [WinUIEssentials](https://github.com/HO-COOH/WinUIEssentials).
