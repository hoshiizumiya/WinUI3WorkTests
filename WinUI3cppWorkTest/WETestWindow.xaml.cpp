#include "pch.h"
#include "WETestWindow.xaml.h"
#include "TaskDataRow.h"
#include "TasksPageViewModel.h"
#if __has_include("WETestWindow.g.cpp")
#include "WETestWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::WinUI3cppWorkTest::implementation
{
    WETestWindow::WETestWindow()
        : m_viewModel(make<TasksPageViewModel>())
    {
        // As in OpenNet, establish the ViewModel before XAML compiles its x:Bind
        // expressions. Loaded then activates it after the ListView is in the tree.
        InitializeComponent();
    }

    winrt::WinUI3cppWorkTest::TasksPageViewModel WETestWindow::ViewModel() const
    {
        return m_viewModel;
    }

    void WETestWindow::Window_Loaded(IInspectable const&, RoutedEventArgs const&)
    {
        m_viewModel.Initialize();
    }

    void WETestWindow::TaskDataRow_Loaded(IInspectable const& sender, RoutedEventArgs const&)
    {
        auto row = sender.try_as<winrt::WinUI3cppWorkTest::TaskDataRow>();
        if (!row)
        {
            return;
        }

        // Keep the same cell-to-column mapping as TasksPage. This changes only
        // Visibility on existing cells; the row's child collection stays fixed.
        std::array<Visibility, 14> const columnVisibility{
            ColName().Visibility(), ColSize().Visibility(), ColProgress().Visibility(),
            ColDownloadSize().Visibility(), ColUploadSize().Visibility(),
            ColumnTotalDownloadSize().Visibility(), ColumnTotalUploadSize().Visibility(),
            ColDLRate().Visibility(), ColULRate().Visibility(), ColRemaining().Visibility(),
            ColAddDate().Visibility(), ColCompletedDate().Visibility(),
            ColShareRatio().Visibility(), ColSeeds().Visibility() };

        auto const children = row.Children();
        auto const count = (std::min)(children.Size(), static_cast<uint32_t>(columnVisibility.size()));
        for (uint32_t index = 0; index < count; ++index)
        {
            children.GetAt(index).Visibility(columnVisibility[index]);
        }
        row.InvalidateMeasure();
        row.InvalidateArrange();
    }
}
