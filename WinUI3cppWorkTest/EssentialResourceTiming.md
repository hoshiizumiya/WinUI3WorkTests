# ProgressBarEx first-use during ListView layout

This sample reduces the failure to one `ListView`, one initially empty observable vector, and one `ProgressBarEx` in the item template. Run only one case per process because `TemplateControlHelper` loads the control's resource dictionary once per process.

## Cases

| Button | Order of operations | Result |
| --- | --- | --- |
| Cold ProgressBarEx in ListView | Show and lay out the empty `ListView`; click; append the first item; synchronously call `UpdateLayout`; the item template constructs the first `ProgressBarEx` during measure. | Reproduces `Child collection must not be modified during measure or arrange`. |
| Prewarm, then populate ListView | Show and lay out the empty `ListView`; click; construct a detached `ProgressBarEx`; append the first item; call `UpdateLayout`. | Does not crash. The resource dictionary is merged before ListView measures the first row. |
| Direct construction | Show and lay out the window; click; construct a `ProgressBarEx` and assign it to a `ContentControl`. | Does not crash. Resource loading and content insertion occur outside ListView measure. |

Restart the app before each case. After one case, the process has already loaded the ProgressBarEx resource dictionary, so a second case would no longer test first use.

`TaskItems` is created as an empty `IObservableVector<IInspectable>` before `InitializeComponent`. XAML binds the `ListView` to that same vector with `x:Bind`. The first item is appended only after the window is visible, so the initial empty-list layout has completed. The cold and prewarm paths both append the same boxed integers and call the same `UpdateLayout`; their only meaningful difference is whether a `ProgressBarEx` was constructed before the item was realized.

The template sets `Percent="50"`. `ProgressBarEx.Value` is normalized to `0.0–1.0`, while `Percent` is `0–100`; using `Value="50"` would mean 5000% before clamping and would not represent a 50% sample.

## What the first-chance stack proves

The failing cold run has this relevant stack, from the layout caller down to the resource append:

```text
DirectUI::ListViewBase::MeasureOverride
  CUIElement::MeasureInternal / CUIElement::Measure
    DirectUI::ModernCollectionBasePanel::MeasureOverride
      DirectUI::ModernCollectionBasePanel::MeasureSpecialElements
        GenerateContainerAtIndexImpl
          CDataTemplate::QueryContentNoRef
            CTemplateContent::Load
              XamlType::CreateInstance(ProgressBarEx)
                ProgressBarEx::ProgressBarEx
                  TemplateControlHelper::TemplateControlHelper
                    XamlResourceHelper::XamlResourceHelper
                      Application.Resources.MergedDictionaries().Append
                        CResourceDictionaryCollection::OnAddToCollection
                          CResourceDictionary::InvalidateImplicitStyles
                            CCoreServices::InvalidateImplicitStylesOnRoots
                              CFrameworkElement::InvalidateImplicitStyles
                                CFrameworkElement::OnStyleChanged
                                  CScrollContentControl::SetValue
                                    CContentControl::SetValue
                                      CUIElement::RemoveChild
                                        CCollection::FailIfLocked
```

The stack therefore identifies two simultaneous operations:

1. The `ItemsStackPanel` is measuring the ListView and creates the first item from its `DataTemplate`. This is the `MeasureOverride → MeasureSpecialElements → CDataTemplate::QueryContentNoRef` portion.
2. During that construction, `XamlResourceHelper` appends the ProgressBarEx dictionary to application resources. The dictionary contains an implicit `Style TargetType="ProgressBarEx"`. WinUI's `CResourceDictionaryCollection::OnAddToCollection` detects that implicit style and invalidates implicit styles on existing roots. Style refresh reaches `CContentControl::SetValue`; when a control template changes, that code removes the old template child. The owning child collection is still locked by the active layout pass, so `CCollection::FailIfLocked` raises the exception.

The source paths corresponding to these frames are [`TemplateControlHelper.hpp`](https://github.com/HO-COOH/WinUIEssentials/blob/master/WinUI3Package/include/TemplateControlHelper.hpp), [`ProgressBarEx_Resource.xaml`](https://github.com/HO-COOH/WinUIEssentials/blob/master/WinUI3Package/ProgressBarEx_Resource.xaml), and WinUI's [`ResourceDictionaryCollection.cpp`](https://github.com/microsoft/microsoft-ui-xaml/blob/main/dxaml/xcp/components/Collection/ResourceDictionaryCollection.cpp), [`ContentControl.cpp`](https://github.com/microsoft/microsoft-ui-xaml/blob/main/dxaml/xcp/core/core/elements/ContentControl.cpp), and [`collect.cpp`](https://github.com/microsoft/microsoft-ui-xaml/blob/main/dxaml/xcp/components/Collection/collect.cpp).

`ProgressBarEx::OnApplyTemplate` is not the child-removal site in this stack. The control constructor causes a global resource update; WinUI's implicit-style refresh then reaches a `ContentControl` and attempts to remove a template child while layout has locked the collection. The `CScrollContentControl` frame identifies the class involved, but this stack alone does not identify which particular visual-tree instance it is.

## Why the two controls do not crash

In the prewarm case, the same dictionary append and implicit-style invalidation run in the button handler before the first item is added. There is no ListView measure pass holding the relevant child collection at that moment. When the item template later constructs its own `ProgressBarEx`, the helper's once-only resource initialization has already completed, so it does not append the dictionary again during measure.

In the direct case, the first constructor also runs in the button handler while the window is idle. Assigning the new control to `DirectHost.Content` then adds it to the visual tree outside ListView measure. The constructor does not cause a child mutation while a collection is locked.

The maintainer's prepopulated-vector example is a valid non-crashing case, but it does not perform this sample's explicit transition from an already laid-out empty ListView to a newly populated ListView. It shows that a `ProgressBarEx` in an item template is not sufficient by itself to trigger the exception. The cold/prewarm comparison isolates the additional condition shown in the stack: first-time global implicit-style invalidation occurs while the ListView is synchronously realizing an item during measure.

## Reproduction steps

1. Start a fresh process and wait for the empty ListView to appear.
2. Click **Cold ProgressBarEx in ListView**. The first item is appended, then `UpdateLayout` synchronously forces item realization and measure.
3. Restart the app and click **Prewarm, then populate ListView**.
4. Restart again and click **Direct construction**.

In Visual Studio, break on the first thrown WinRT exception. The useful frames are `CCollection::FailIfLocked`, `CUIElement::RemoveChild`, `CContentControl::SetValue`, `CFrameworkElement::OnStyleChanged`, `CResourceDictionaryCollection::OnAddToCollection`, and `XamlResourceHelper::XamlResourceHelper`. The later unhandled-exception callback does not show the original cause as clearly.
