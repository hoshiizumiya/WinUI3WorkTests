using Microsoft.UI;
using Microsoft.UI.Composition;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Media;

namespace WinUI3csWorkTest;

/// <summary>
/// A no-op/transparent SystemBackdrop used to isolate the effect of making the
/// XAML island transparent from Mica/DesktopAcrylic's own visual material.
/// </summary>
public sealed partial class TransparentTestBackdrop : SystemBackdrop
{
    private Windows.UI.Composition.Compositor? _compositor;
    private Windows.UI.Composition.CompositionColorBrush? _brush;

    protected override void OnTargetConnected(ICompositionSupportsSystemBackdrop target, XamlRoot xamlRoot)
    {
        _compositor ??= new Windows.UI.Composition.Compositor();
        _brush ??= _compositor.CreateColorBrush(Colors.Transparent);
        target.SystemBackdrop = _brush;
    }

    protected override void OnTargetDisconnected(ICompositionSupportsSystemBackdrop target)
    {
        target.SystemBackdrop = null;
        _brush?.Dispose();
        _brush = null;
        _compositor?.Dispose();
        _compositor = null;
    }
}
