#pragma once

#include "WETestWindow.g.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow>
    {
        WETestWindow();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> TaskItems() const;

        void ColdListButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void PrewarmButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void DirectButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        void RunList(bool prewarm);

        bool m_hasRun{};
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> m_taskItems{ nullptr };
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow, implementation::WETestWindow>
    {
    };
}
