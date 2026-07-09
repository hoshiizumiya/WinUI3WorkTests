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

        void Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void MenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct DevWindow : DevWindowT<DevWindow, implementation::DevWindow>
    {
    };
}
