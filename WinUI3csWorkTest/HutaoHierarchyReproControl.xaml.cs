using Microsoft.UI.Xaml.Controls;

namespace WinUI3csWorkTest;

public sealed partial class HutaoHierarchyReproControl : UserControl
{
    public HutaoHierarchyReproControl()
    {
        InitializeComponent();
    }

    public ScrollViewer Scroller => OuterScroller;
}
