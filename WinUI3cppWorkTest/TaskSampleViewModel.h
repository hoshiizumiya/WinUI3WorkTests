#pragma once

#include "TaskSampleViewModel.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct TaskSampleViewModel : TaskSampleViewModelT<TaskSampleViewModel>
    {
        TaskSampleViewModel(winrt::hstring const& name, int32_t index);

        winrt::hstring Name() const { return m_name; }
        winrt::hstring Size() const { return m_size; }
        winrt::hstring Progress() const { return m_progress; }
        winrt::hstring DownloadSize() const { return m_downloadSize; }
        winrt::hstring UploadSize() const { return m_uploadSize; }
        winrt::hstring TotalDownloadSize() const { return m_totalDownloadSize; }
        winrt::hstring TotalUploadSize() const { return m_totalUploadSize; }
        winrt::hstring DownloadRate() const { return m_downloadRate; }
        winrt::hstring UploadRate() const { return m_uploadRate; }
        winrt::hstring Remaining() const { return m_remaining; }
        winrt::hstring AddDate() const { return m_addDate; }
        winrt::hstring CompletedDate() const { return m_completedDate; }
        winrt::hstring ShareRatio() const { return m_shareRatio; }
        winrt::hstring Seeds() const { return m_seeds; }
        winrt::hstring Transport() const { return m_transport; }
        double ProgressPercent() const noexcept { return m_progressPercent; }
        winrt::Windows::UI::Color ProgressHighColor() const noexcept { return m_progressHighColor; }
        winrt::Windows::UI::Color ProgressBaseColor() const noexcept { return { 0, 0, 0, 0 }; }
        winrt::Microsoft::UI::Xaml::Visibility ProgressRowEffectVisibility() const noexcept { return m_rowEffectVisibility; }
        winrt::Microsoft::UI::Xaml::Visibility ProgressColumnEffectVisibility() const noexcept { return m_columnEffectVisibility; }

    private:
        winrt::hstring m_name;
        winrt::hstring m_size;
        winrt::hstring m_progress;
        winrt::hstring m_downloadSize;
        winrt::hstring m_uploadSize;
        winrt::hstring m_totalDownloadSize;
        winrt::hstring m_totalUploadSize;
        winrt::hstring m_downloadRate;
        winrt::hstring m_uploadRate;
        winrt::hstring m_remaining;
        winrt::hstring m_addDate;
        winrt::hstring m_completedDate;
        winrt::hstring m_shareRatio;
        winrt::hstring m_seeds;
        winrt::hstring m_transport;
        double m_progressPercent{};
        winrt::Windows::UI::Color m_progressHighColor{};
        winrt::Microsoft::UI::Xaml::Visibility m_rowEffectVisibility{ winrt::Microsoft::UI::Xaml::Visibility::Visible };
        winrt::Microsoft::UI::Xaml::Visibility m_columnEffectVisibility{ winrt::Microsoft::UI::Xaml::Visibility::Visible };
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct TaskSampleViewModel : TaskSampleViewModelT<TaskSampleViewModel, implementation::TaskSampleViewModel>
    {
    };
}
