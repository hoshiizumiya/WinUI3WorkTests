#include "pch.h"
#include "TaskSampleViewModel.h"
#if __has_include("TaskSampleViewModel.g.cpp")
#include "TaskSampleViewModel.g.cpp"
#endif

namespace winrt::WinUI3cppWorkTest::implementation
{
    TaskSampleViewModel::TaskSampleViewModel(winrt::hstring const& name) : m_name(name)
    {
    }

    winrt::hstring TaskSampleViewModel::Name() const
    {
        return m_name;
    }
}
