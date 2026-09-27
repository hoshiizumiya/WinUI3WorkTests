using Microsoft.UI;
using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Imaging;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.UI;

namespace WinUI3csWorkTest;

public sealed partial class MainWindow : Window
{
    private const uint SpiGetDesktopWallpaper = 0x0073;
    private readonly DispatcherQueueTimer _scrollTimer;
    private double _scrollDirection = 1.0;

    public MainWindow()
    {
        InitializeComponent();
        ExtendsContentIntoTitleBar = true;

        _scrollTimer = DispatcherQueue.CreateTimer();
        _scrollTimer.Interval = TimeSpan.FromMilliseconds(16);
        _scrollTimer.IsRepeating = true;
        _scrollTimer.Tick += ScrollTimer_Tick;

        ConfigureEffectCards();
        Activated += MainWindow_Activated;
    }

    private async void MainWindow_Activated(object sender, WindowActivatedEventArgs args)
    {
        Activated -= MainWindow_Activated;
        await LoadWallpaperAndPopulateRowsAsync();

        if (AutoScrollToggle.IsOn)
        {
            _scrollTimer.Start();
        }
    }

    private void ConfigureEffectCards()
    {
        RawBackdropCard.Background = new BackdropEffectBrush(BackdropEffectKind.RawBackdrop);

        AcrylicCard.Background = new AcrylicBrush
        {
            TintColor = Colors.Gray,
            TintOpacity = 0.10,
            TintLuminosityOpacity = 0.08,
            FallbackColor = Colors.Transparent
        };

        BlurCard.Background = new BackdropEffectBrush(BackdropEffectKind.GaussianBlur);
        SaturationCard.Background = new BackdropEffectBrush(BackdropEffectKind.Saturation);
        HueCard.Background = new BackdropEffectBrush(BackdropEffectKind.HueRotation);
        InvertCard.Background = new BackdropEffectBrush(BackdropEffectKind.Invert);
        SepiaCard.Background = new BackdropEffectBrush(BackdropEffectKind.Sepia);
        ExposureCard.Background = new BackdropEffectBrush(BackdropEffectKind.Exposure);
        GrayscaleCard.Background = new BackdropEffectBrush(BackdropEffectKind.Grayscale);
    }

    private async Task LoadWallpaperAndPopulateRowsAsync()
    {
        BitmapImage? wallpaper = null;
        var wallpaperPath = FindCurrentWallpaperPath();

        if (wallpaperPath is not null)
        {
            try
            {
                var file = await StorageFile.GetFileFromPathAsync(wallpaperPath);
                using var stream = await file.OpenReadAsync();
                wallpaper = new BitmapImage();
                await wallpaper.SetSourceAsync(stream);
                WallpaperStatusText.Text = $"Wallpaper: {wallpaperPath}";
            }
            catch (Exception ex)
            {
                WallpaperStatusText.Text = $"Wallpaper load failed: {ex.Message}";
            }
        }
        else
        {
            WallpaperStatusText.Text = "Current wallpaper path was not available; using the fallback pattern.";
        }

        PopulateBackdropRows(wallpaper);
    }

    private void PopulateBackdropRows(BitmapImage? wallpaper)
    {
        BackdropSourcePanel.Children.Clear();

        for (var index = 0; index < 24; index++)
        {
            var row = new Border
            {
                Height = 320,
                CornerRadius = new CornerRadius(18),
                BorderBrush = new SolidColorBrush(Color.FromArgb(180, 255, 255, 255)),
                BorderThickness = new Thickness(1),
                Background = wallpaper is null
                    ? CreateFallbackBrush(index)
                    : new ImageBrush
                    {
                        ImageSource = wallpaper,
                        Stretch = Stretch.UniformToFill,
                        AlignmentX = AlignmentX.Center,
                        AlignmentY = index % 2 == 0 ? AlignmentY.Top : AlignmentY.Bottom
                    }
            };

            var grid = new Grid
            {
                Padding = new Thickness(28)
            };

            var marker = new Border
            {
                HorizontalAlignment = HorizontalAlignment.Left,
                VerticalAlignment = VerticalAlignment.Top,
                Padding = new Thickness(16, 10, 16, 10),
                Background = new SolidColorBrush(index % 2 == 0
                    ? Color.FromArgb(220, 20, 20, 20)
                    : Color.FromArgb(220, 240, 240, 240)),
                CornerRadius = new CornerRadius(10)
            };

            marker.Child = new TextBlock
            {
                Text = $"MOVING BACKDROP {index:00}",
                FontSize = 34,
                FontWeight = Microsoft.UI.Text.FontWeights.Bold,
                Foreground = new SolidColorBrush(index % 2 == 0 ? Colors.White : Colors.Black)
            };

            var number = new TextBlock
            {
                Text = index.ToString("00"),
                HorizontalAlignment = HorizontalAlignment.Right,
                VerticalAlignment = VerticalAlignment.Bottom,
                FontFamily = new FontFamily("Consolas"),
                FontSize = 108,
                FontWeight = Microsoft.UI.Text.FontWeights.Bold,
                Foreground = new SolidColorBrush(Color.FromArgb(225, 255, 255, 255))
            };

            var controls = new StackPanel
            {
                Orientation = Orientation.Horizontal,
                HorizontalAlignment = HorizontalAlignment.Left,
                VerticalAlignment = VerticalAlignment.Bottom,
                Spacing = 12
            };
            controls.Children.Add(new Button { Content = $"Button {index:00}" });
            controls.Children.Add(new ToggleSwitch { Header = "High-contrast moving XAML", IsOn = index % 2 == 0 });

            grid.Children.Add(marker);
            grid.Children.Add(number);
            grid.Children.Add(controls);
            row.Child = grid;
            BackdropSourcePanel.Children.Add(row);
        }
    }

    private static Brush CreateFallbackBrush(int index)
    {
        var brush = new LinearGradientBrush
        {
            StartPoint = new Windows.Foundation.Point(0, 0),
            EndPoint = new Windows.Foundation.Point(1, 1)
        };

        if (index % 2 == 0)
        {
            brush.GradientStops.Add(new GradientStop { Color = Color.FromArgb(255, 35, 96, 180), Offset = 0 });
            brush.GradientStops.Add(new GradientStop { Color = Color.FromArgb(255, 210, 60, 120), Offset = 1 });
        }
        else
        {
            brush.GradientStops.Add(new GradientStop { Color = Color.FromArgb(255, 20, 150, 110), Offset = 0 });
            brush.GradientStops.Add(new GradientStop { Color = Color.FromArgb(255, 135, 70, 200), Offset = 1 });
        }

        return brush;
    }

    private void ScrollTimer_Tick(object? sender, object args)
    {
        var maximum = BackdropSourceScroller.ScrollableHeight;
        if (maximum <= 0)
        {
            return;
        }

        var next = BackdropSourceScroller.VerticalOffset + ScrollSpeedSlider.Value * _scrollDirection;

        if (next >= maximum)
        {
            next = maximum;
            _scrollDirection = -1.0;
        }
        else if (next <= 0)
        {
            next = 0;
            _scrollDirection = 1.0;
        }

        BackdropSourceScroller.ChangeView(null, next, null, true);
    }

    private void AutoScrollToggle_Toggled(object sender, RoutedEventArgs e)
    {
        if (_scrollTimer is null)
        {
            return;
        }

        if (AutoScrollToggle.IsOn)
        {
            _scrollTimer.Start();
        }
        else
        {
            _scrollTimer.Stop();
        }
    }

    private void ResetScroll_Click(object sender, RoutedEventArgs e)
    {
        _scrollDirection = 1.0;
        BackdropSourceScroller.ChangeView(null, 0, null, true);
    }

    private void BackdropSourceScroller_ViewChanged(object sender, ScrollViewerViewChangedEventArgs e)
    {
        ScrollStateText.Text = $"Offset: {BackdropSourceScroller.VerticalOffset:F1} / {BackdropSourceScroller.ScrollableHeight:F1}";
    }

    private static string? FindCurrentWallpaperPath()
    {
        var buffer = new StringBuilder(32768);
        if (SystemParametersInfo(SpiGetDesktopWallpaper, (uint)buffer.Capacity, buffer, 0))
        {
            var directPath = buffer.ToString();
            if (!string.IsNullOrWhiteSpace(directPath) && File.Exists(directPath))
            {
                return directPath;
            }
        }

        var themesDirectory = Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
            "Microsoft",
            "Windows",
            "Themes");

        var transcodedWallpaper = Path.Combine(themesDirectory, "TranscodedWallpaper");
        if (File.Exists(transcodedWallpaper))
        {
            return transcodedWallpaper;
        }

        var cachedFiles = Path.Combine(themesDirectory, "CachedFiles");
        if (Directory.Exists(cachedFiles))
        {
            string? newest = null;
            DateTime newestWriteTime = DateTime.MinValue;

            foreach (var path in Directory.EnumerateFiles(cachedFiles))
            {
                var writeTime = File.GetLastWriteTimeUtc(path);
                if (writeTime > newestWriteTime)
                {
                    newest = path;
                    newestWriteTime = writeTime;
                }
            }

            return newest;
        }

        return null;
    }

    [DllImport("user32.dll", CharSet = CharSet.Unicode)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool SystemParametersInfo(uint uiAction, uint uiParam, StringBuilder pvParam, uint fWinIni);
}
