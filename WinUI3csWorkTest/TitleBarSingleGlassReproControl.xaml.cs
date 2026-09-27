using Microsoft.UI.Xaml.Controls;

namespace WinUI3csWorkTest;

public sealed partial class TitleBarSingleGlassReproControl : UserControl
{
    public TitleBarSingleGlassReproControl()
    {
        InitializeComponent();
    }

    public ScrollViewer Scroller => RootScroller;
}
