#include "pch.h"
#include "TasksPageViewModel.h"
#include "TaskSampleViewModel.h"
#if __has_include("TasksPageViewModel.g.cpp")
#include "TasksPageViewModel.g.cpp"
#endif

namespace winrt::WinUI3cppWorkTest::implementation
{
    TasksPageViewModel::TasksPageViewModel()
        : m_tasks(winrt::single_threaded_observable_vector<winrt::Windows::Foundation::IInspectable>())
    {
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable>
    TasksPageViewModel::FilteredTasks() const
    {
        return m_tasks;
    }

    void TasksPageViewModel::Initialize()
    {
        if (m_initialized)
        {
            return;
        }
        m_initialized = true;

        // Match OpenNet's Loaded-time activation. The XAML ItemsSource is bound
        // already, so these collection notifications schedule ordinary layout.
        for (int32_t index = 0; index < 500; ++index)
        {
            m_tasks.Append(winrt::make<winrt::WinUI3cppWorkTest::implementation::TaskSampleViewModel>(
                L"OpenNet sample task " + winrt::to_hstring(index + 1), index));
        }
    }
}
