#pragma once

#include "TasksPageViewModel.g.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct TasksPageViewModel : TasksPageViewModelT<TasksPageViewModel>
    {
        TasksPageViewModel();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> FilteredTasks() const;
        void Initialize();

    private:
        bool m_initialized{};
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> m_tasks{ nullptr };
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct TasksPageViewModel : TasksPageViewModelT<TasksPageViewModel, implementation::TasksPageViewModel>
    {
    };
}
