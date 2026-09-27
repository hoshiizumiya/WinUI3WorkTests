using Microsoft.Graphics.Canvas.Effects;
using Microsoft.UI.Composition;
using Microsoft.UI.Xaml.Media;
using System.Numerics;
using Windows.Graphics.Effects;

namespace WinUI3csWorkTest;

internal enum BackdropEffectKind
{
    RawBackdrop,
    GaussianBlur,
    Saturation,
    HueRotation,
    Invert,
    Sepia,
    Exposure,
    Transform2D
}

internal sealed class BackdropEffectBrush : XamlCompositionBrushBase
{
    private readonly BackdropEffectKind _kind;
    private CompositionBackdropBrush? _backdropBrush;
    private CompositionEffectBrush? _effectBrush;

    public BackdropEffectBrush(BackdropEffectKind kind)
    {
        _kind = kind;
    }

    protected override void OnConnected()
    {
        if (CompositionBrush is not null)
        {
            return;
        }

        var compositor = CompositionTarget.GetCompositorForCurrentThread();
        _backdropBrush = compositor.CreateBackdropBrush();

        if (_kind == BackdropEffectKind.RawBackdrop)
        {
            CompositionBrush = _backdropBrush;
            return;
        }

        var source = new CompositionEffectSourceParameter("Backdrop");
        IGraphicsEffect effect = CreateEffect(_kind, source);
        var factory = compositor.CreateEffectFactory(effect);
        _effectBrush = factory.CreateBrush();
        _effectBrush.SetSourceParameter("Backdrop", _backdropBrush);
        CompositionBrush = _effectBrush;
    }

    protected override void OnDisconnected()
    {
        CompositionBrush = null;
        _effectBrush = null;
        _backdropBrush = null;
    }

    private static IGraphicsEffect CreateEffect(BackdropEffectKind kind, IGraphicsEffectSource source)
    {
        return kind switch
        {
            BackdropEffectKind.GaussianBlur => new GaussianBlurEffect
            {
                Name = "GaussianBlur",
                Source = source,
                BlurAmount = 28.0f,
                BorderMode = EffectBorderMode.Hard,
                Optimization = EffectOptimization.Balanced
            },
            BackdropEffectKind.Saturation => new SaturationEffect
            {
                Name = "Saturation",
                Source = source,
                Saturation = 0.0f
            },
            BackdropEffectKind.HueRotation => new HueRotationEffect
            {
                Name = "HueRotation",
                Source = source,
                Angle = 1.8f
            },
            BackdropEffectKind.Invert => new InvertEffect
            {
                Name = "Invert",
                Source = source
            },
            BackdropEffectKind.Sepia => new SepiaEffect
            {
                Name = "Sepia",
                Source = source,
                Intensity = 1.0f
            },
            BackdropEffectKind.Exposure => new ExposureEffect
            {
                Name = "Exposure",
                Source = source,
                Exposure = 1.25f
            },
            BackdropEffectKind.Transform2D => new Transform2DEffect
            {
                Name = "Transform2D",
                Source = source,
                TransformMatrix = Matrix3x2.CreateTranslation(18.0f, 12.0f)
            },
            _ => throw new ArgumentOutOfRangeException(nameof(kind), kind, null)
        };
    }
}
