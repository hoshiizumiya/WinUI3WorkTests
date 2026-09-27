using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
using Windows.UI;

namespace WinUI3csWorkTest;

public sealed class EffectCardLabel : Grid
{
    public string Title
    {
        get => (string)GetValue(TitleProperty);
        set => SetValue(TitleProperty, value);
    }

    public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
        nameof(Title),
        typeof(string),
        typeof(EffectCardLabel),
        new PropertyMetadata(string.Empty, OnTextChanged));

    public string Subtitle
    {
        get => (string)GetValue(SubtitleProperty);
        set => SetValue(SubtitleProperty, value);
    }

    public static readonly DependencyProperty SubtitleProperty = DependencyProperty.Register(
        nameof(Subtitle),
        typeof(string),
        typeof(EffectCardLabel),
        new PropertyMetadata(string.Empty, OnTextChanged));

    private readonly TextBlock _title;
    private readonly TextBlock _subtitle;

    public EffectCardLabel()
    {
        Padding = new Thickness(14);

        var panel = new StackPanel
        {
            Spacing = 3,
            HorizontalAlignment = HorizontalAlignment.Left,
            VerticalAlignment = VerticalAlignment.Top
        };

        var labelBackground = new Border
        {
            Padding = new Thickness(10, 7, 10, 7),
            Background = new SolidColorBrush(Color.FromArgb(205, 16, 16, 16)),
            CornerRadius = new CornerRadius(8)
        };

        _title = new TextBlock
        {
            FontSize = 15,
            FontWeight = Microsoft.UI.Text.FontWeights.SemiBold,
            Foreground = new SolidColorBrush(Microsoft.UI.Colors.White)
        };

        _subtitle = new TextBlock
        {
            FontSize = 11,
            Opacity = 0.75,
            Foreground = new SolidColorBrush(Microsoft.UI.Colors.White)
        };

        panel.Children.Add(_title);
        panel.Children.Add(_subtitle);
        labelBackground.Child = panel;
        Children.Add(labelBackground);

        RefreshText();
    }

    private static void OnTextChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        ((EffectCardLabel)d).RefreshText();
    }

    private void RefreshText()
    {
        _title.Text = Title;
        _subtitle.Text = Subtitle;
    }
}
