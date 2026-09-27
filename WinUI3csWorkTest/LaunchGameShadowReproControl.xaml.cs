using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace WinUI3csWorkTest;

public sealed partial class LaunchGameShadowReproControl : UserControl
{
    public LaunchGameShadowReproControl()
    {
        InitializeComponent();
    }

    public ScrollViewer Scroller => RootScroller;

    private void ToggleReposition_Click(object sender, RoutedEventArgs e)
    {
        RepositionBump.Visibility = RepositionBump.Visibility == Visibility.Visible
            ? Visibility.Collapsed
            : Visibility.Visible;
    }
}
