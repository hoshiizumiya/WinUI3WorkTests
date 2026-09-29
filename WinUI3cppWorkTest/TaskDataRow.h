#pragma once

#include "TaskDataRow.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct TaskDataRow : TaskDataRowT<TaskDataRow>
    {
        TaskDataRow() = default;

        winrt::Windows::Foundation::Size MeasureOverride(winrt::Windows::Foundation::Size availableSize);
        winrt::Windows::Foundation::Size ArrangeOverride(winrt::Windows::Foundation::Size finalSize);
        uint32_t MeasurePassCount() const noexcept { return m_measurePassCount; }
        uint32_t ArrangePassCount() const noexcept { return m_arrangePassCount; }

    private:
        uint32_t m_measurePassCount{};
        uint32_t m_arrangePassCount{};
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct TaskDataRow : TaskDataRowT<TaskDataRow, implementation::TaskDataRow>
    {
    };
}
