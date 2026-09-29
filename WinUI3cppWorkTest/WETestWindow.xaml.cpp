#include "pch.h"
#include "WETestWindow.xaml.h"
#if __has_include("WETestWindow.g.cpp")
#include "WETestWindow.g.cpp"
#endif

#include <winrt/WinUI3Package.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::WinUI3cppWorkTest::implementation
{
    WETestWindow::WETestWindow()
        : m_taskItems(single_threaded_observable_vector<IInspectable>())
    {
        InitializeComponent();
    }

    Windows::Foundation::Collections::IObservableVector<IInspectable> WETestWindow::TaskItems() const
    {
        return m_taskItems;
    }

    void WETestWindow::RunList(bool prewarm)
    {
        if (m_hasRun)
        {
            return;
        }

        m_hasRun = true;
        ColdListButton().IsEnabled(false);
        PrewarmButton().IsEnabled(false);
        DirectButton().IsEnabled(false);

        if (prewarm)
        {
            Status().Text(L"Constructing ProgressBarEx outside ListView layout, then adding the first item.");

            // This first construction loads the global implicit-style dictionary
            // while no ListView measure or arrange pass is running.
            WinUI3Package::ProgressBarEx prewarmed;
            prewarmed.Height(20);
            prewarmed.Percent(50);
        }
        else
        {
            Status().Text(L"Adding the first item now. Its DataTemplate will construct the first ProgressBarEx during ListView measure.");
        }

        // The ListView was already laid out while this observable vector was
        // empty. Force its next measure synchronously so the first item template
        // is realized before this handler returns.
        m_taskItems.Append(box_value(1));
        TasksList().InvalidateMeasure();
        TasksList().UpdateLayout();

        for (int32_t index = 2; index <= 7; ++index)
        {
            m_taskItems.Append(box_value(index));
        }

        Status().Text(prewarm
            ? L"Prewarm case completed: the resource dictionary was loaded before ListView realized its first row."
            : L"Cold case completed. If the first-use layout failure occurs, the debugger stops during the forced ListView measure above.");
    }

    void WETestWindow::ColdListButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        RunList(false);
    }

    void WETestWindow::PrewarmButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        RunList(true);
    }

    void WETestWindow::DirectButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        if (m_hasRun)
        {
            return;
        }

        m_hasRun = true;
        ColdListButton().IsEnabled(false);
        PrewarmButton().IsEnabled(false);
        DirectButton().IsEnabled(false);
        Status().Text(L"Constructing ProgressBarEx outside ListView layout and placing it in a ContentControl.");

        WinUI3Package::ProgressBarEx progress;
        progress.Height(20);
        progress.Percent(50);
        DirectHost().Content(progress);

        Status().Text(L"Direct case completed: resource loading and ContentControl insertion ran outside ListView measure.");
    }
}
