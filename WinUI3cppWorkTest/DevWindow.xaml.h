#pragma once

#include "DevWindow.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct DevWindow : DevWindowT<DevWindow>
    {
    public:
        DevWindow();

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
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct DevWindow : DevWindowT<DevWindow, implementation::DevWindow>
    {
    };
}
