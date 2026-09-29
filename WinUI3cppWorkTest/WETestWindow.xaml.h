#pragma once

#include "WETestWindow.g.h"
#include <winrt/WinUI3cppWorkTest.h>
#include <array>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow>
    {
        WETestWindow();
        winrt::WinUI3cppWorkTest::TasksPageViewModel ViewModel() const;

        void Window_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void TaskDataRow_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        winrt::WinUI3cppWorkTest::TasksPageViewModel m_viewModel{ nullptr };
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow, implementation::WETestWindow>
    {
    };
}
