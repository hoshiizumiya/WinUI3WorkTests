#pragma once

#include "TaskSampleViewModel.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct TaskSampleViewModel : TaskSampleViewModelT<TaskSampleViewModel>
    {
        explicit TaskSampleViewModel(winrt::hstring const& name);
        winrt::hstring Name() const;
        double ProgressPercent() const noexcept { return 38.0; }
        winrt::Windows::UI::Color ProgressHighColor() const noexcept { return { 255, 0, 255, 0 }; }
        winrt::Windows::UI::Color ProgressBaseColor() const noexcept { return { 0, 0, 0, 0 }; }
        winrt::Microsoft::UI::Xaml::Visibility ProgressRowEffectVisibility() const noexcept { return winrt::Microsoft::UI::Xaml::Visibility::Visible; }
        winrt::Microsoft::UI::Xaml::Visibility ProgressColumnEffectVisibility() const noexcept { return winrt::Microsoft::UI::Xaml::Visibility::Visible; }

    private:
        winrt::hstring m_name;
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct TaskSampleViewModel : TaskSampleViewModelT<TaskSampleViewModel, implementation::TaskSampleViewModel>
    {
    };
}
