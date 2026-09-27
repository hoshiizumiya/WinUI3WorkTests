using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace WinUI3csWorkTest;

public sealed partial class NativeBackdropSystemBackdropReproControl : UserControl
{
    public NativeBackdropSystemBackdropReproControl()
    {
        InitializeComponent();

        RawBackdropCard.Background = new BackdropEffectBrush(BackdropEffectKind.RawBackdrop);
        GaussianBlurCard.Background = new BackdropEffectBrush(BackdropEffectKind.GaussianBlur);
        InvertCard.Background = new BackdropEffectBrush(BackdropEffectKind.Invert);
        SaturationCard.Background = new BackdropEffectBrush(BackdropEffectKind.Saturation);
        HueCard.Background = new BackdropEffectBrush(BackdropEffectKind.HueRotation);
    }

    public ScrollViewer Scroller => RootScroller;

    private void ToggleReposition_Click(object sender, RoutedEventArgs e)
    {
        RepositionSpacer.Visibility = RepositionSpacer.Visibility == Visibility.Visible
            ? Visibility.Collapsed
            : Visibility.Visible;
    }
}
