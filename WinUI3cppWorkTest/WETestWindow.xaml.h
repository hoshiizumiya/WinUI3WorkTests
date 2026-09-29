#pragma once

#include "WETestWindow.g.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow>
    {
        WETestWindow();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> TaskItems() const;

        void RunColdList_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void RunWarmList_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void RunDirect_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void SampleItem_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void TaskDataRow_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void ProgressControl_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        void RunList(bool warm);
        void AppendLog(winrt::hstring const& message);
        winrt::Microsoft::UI::Xaml::UIElement CreateSelectedControl();
        winrt::hstring SelectedControlName();
        uint32_t DictionaryCount() const;

        bool m_hasRun{};
        uint32_t m_loadedCount{};
        uint32_t m_dataRowLoadedCount{};
        uint32_t m_progressLoadedCount{};
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> m_taskItems{ nullptr };
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow, implementation::WETestWindow>
    {
    };
}
