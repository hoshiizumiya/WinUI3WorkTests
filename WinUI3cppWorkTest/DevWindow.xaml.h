#pragma once

#include "DevWindow.g.h"

#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct DevWindow : DevWindowT<DevWindow>
    {
    public:
        DevWindow();

        void ButtonReload_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ButtonRecreate_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ButtonRecreateDispatch_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

        bool BindTestButton();

        double TestDouble();
        void TestDouble(double value);

        bool IsChecked(Microsoft::UI::Xaml::Controls::Primitives::ToggleButton const& button);

        void Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void MenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

        winrt::fire_and_forget SourceGrid_DragStarting(
            Microsoft::UI::Xaml::UIElement const& sender,
            Microsoft::UI::Xaml::DragStartingEventArgs const& args);

        void TargetTextBox_DragEnter(
            Windows::Foundation::IInspectable const& sender,
            Microsoft::UI::Xaml::DragEventArgs const& args);

        void TargetTextBox_DragLeave(
            Windows::Foundation::IInspectable const& sender,
            Microsoft::UI::Xaml::DragEventArgs const& args);

        winrt::fire_and_forget TargetTextBox_Drop(
            Windows::Foundation::IInspectable const& sender,
            Microsoft::UI::Xaml::DragEventArgs const& args);

    private:
        static constexpr wchar_t InsideStateName[] = L"Inside";
        static constexpr wchar_t OutsideStateName[] = L"Outside";
        static constexpr wchar_t DropHereText[] = L"Drop here";

        winrt::Microsoft::UI::Dispatching::DispatcherQueue m_queue{ nullptr };
        winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer m_timer{ nullptr };
        winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSource m_svgImageSource{ nullptr };

        static winrt::Windows::Foundation::IAsyncAction LoadEmbeddedImage(
            winrt::Microsoft::UI::Xaml::Media::Imaging::SvgImageSource svgImageSource,
            winrt::Microsoft::UI::Xaml::Controls::Image image = nullptr);
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct DevWindow : DevWindowT<DevWindow, implementation::DevWindow>
    {
    };
}
