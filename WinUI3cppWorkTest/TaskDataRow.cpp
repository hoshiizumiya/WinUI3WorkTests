#include "pch.h"
#include <algorithm>
#include <array>
#include <numeric>
#include "TaskDataRow.h"
#if __has_include("TaskDataRow.g.cpp")
#include "TaskDataRow.g.cpp"
#endif

namespace winrt::WinUI3cppWorkTest::implementation
{
    namespace
    {
        constexpr std::array<double, 14> ColumnWidths{ 420.0, 100.0, 110.0, 110.0, 110.0, 110.0, 100.0, 100.0, 120.0, 150.0, 150.0, 80.0, 90.0, 90.0 };
        constexpr double ColumnSpacing = 16.0;
        constexpr double RowWidth = 1948.0;
    }

    winrt::Windows::Foundation::Size TaskDataRow::MeasureOverride(winrt::Windows::Foundation::Size availableSize)
    {
        ++m_measurePassCount;
        auto const children = Children();
        auto const count = std::min<uint32_t>(children.Size(), static_cast<uint32_t>(ColumnWidths.size()));
        auto const totalWidth = std::accumulate(ColumnWidths.begin(), ColumnWidths.end(), 0.0) + ColumnSpacing * (ColumnWidths.size() - 1);
        double maxHeight = 0.0;

        for (uint32_t index = 0; index < count; ++index)
        {
            children.GetAt(index).Measure({ static_cast<float>(ColumnWidths[index]), availableSize.Height });
            maxHeight = std::max<double>(maxHeight, children.GetAt(index).DesiredSize().Height);
        }

        return { static_cast<float>((std::max)(RowWidth, totalWidth)), static_cast<float>(maxHeight) };
    }

    winrt::Windows::Foundation::Size TaskDataRow::ArrangeOverride(winrt::Windows::Foundation::Size finalSize)
    {
        ++m_arrangePassCount;
        auto const children = Children();
        auto const count = std::min<uint32_t>(children.Size(), static_cast<uint32_t>(ColumnWidths.size()));
        double x = 0.0;

        for (uint32_t index = 0; index < count; ++index)
        {
            auto const width = ColumnWidths[index];
            children.GetAt(index).Arrange({ static_cast<float>(x), 0.0f, static_cast<float>(width), finalSize.Height });
            x += width + ColumnSpacing;
        }

        return finalSize;
    }
}
