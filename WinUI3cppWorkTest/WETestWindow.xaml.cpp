#include "pch.h"
#include "WETestWindow.xaml.h"
#include "TaskDataRow.h"
#include "TaskSampleViewModel.h"
#if __has_include("WETestWindow.g.cpp")
#include "WETestWindow.g.cpp"
#endif

#include <winrt/WinUI3Package.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::WinUI3cppWorkTest::implementation
{
    WETestWindow::WETestWindow()
    {
        // OpenNet binds a visible ListView before the page view model populates
        // its collection after Loaded. Keep that empty-then-populate lifecycle.
        m_taskItems = single_threaded_observable_vector<IInspectable>();
        InitializeComponent();
        AppendLog(L"Start: merged dictionaries = " + to_hstring(DictionaryCount()));
    }

    Windows::Foundation::Collections::IObservableVector<IInspectable> WETestWindow::TaskItems() const
    {
        return m_taskItems;
    }

    uint32_t WETestWindow::DictionaryCount() const
    {
        return Application::Current().Resources().MergedDictionaries().Size();
    }

    void WETestWindow::AppendLog(hstring const& message)
    {
        std::wstring log{ Diagnostics().Text().c_str() };
        log += L"\n";
        log += message.c_str();
        Diagnostics().Text(hstring{ log });
    }

    hstring WETestWindow::SelectedControlName()
    {
        switch (ControlChoice().SelectedIndex())
        {
        case 0: return L"ProgressBarEx";
        case 1: return L"SettingsCard";
        case 2: return L"SettingsExpander";
        case 3: return L"GroupBox";
        default: throw hresult_invalid_argument(L"Choose a control first.");
        }
    }

    UIElement WETestWindow::CreateSelectedControl()
    {
        switch (ControlChoice().SelectedIndex())
        {
        case 0:
        {
            WinUI3Package::ProgressBarEx progress;
            progress.Height(28);
            progress.Percent(38);
            return progress;
        }
        case 1:
        {
            WinUI3Package::SettingsCard card;
            card.Header(box_value(hstring{ L"Direct creation" }));
            return card;
        }
        case 2:
        {
            WinUI3Package::SettingsExpander expander;
            expander.Header(box_value(hstring{ L"Direct creation" }));
            return expander;
        }
        case 3:
        {
            WinUI3Package::GroupBox group;
            group.Header(L"Direct creation");
            group.Content(box_value(hstring{ L"Group content" }));
            return group;
        }
        default: throw hresult_invalid_argument(L"Choose a control first.");
        }
    }

    void WETestWindow::RunList(bool warm)
    {
        if (m_hasRun) return;
        m_hasRun = true;
        ControlChoice().IsEnabled(false);
        ColdListButton().IsEnabled(false);
        WarmListButton().IsEnabled(false);
        DirectButton().IsEnabled(false);

        auto const before = DictionaryCount();
        AppendLog(warm ? L"ProgressBarEx / prewarm then populate visible ListView" : L"ProgressBarEx / cold populate visible ListView");
        AppendLog(L"Before first construction: " + to_hstring(before));

        if (warm)
        {
            // Load this control's global resources before XAML realizes rows.
            WinUI3Package::ProgressBarEx prewarmed;
            AppendLog(L"After ProgressBarEx construction in Click: " + to_hstring(DictionaryCount()));
        }

        // The ListView and its XAML-fixed template have already completed an
        // empty layout. Populating only the bound collection mirrors OpenNet's
        // Loaded-time view-model activation without replacing ItemTemplate.
        // Add a single item first, then force the visible ListView through its
        // own measure/arrange cycle while the first ProgressBarEx is still cold.
        m_taskItems.Append(make<TaskSampleViewModel>(L"Task 0"));
        AppendLog(L"After first item notification, before forced layout: dictionaries = " + to_hstring(DictionaryCount()));
        SamplesList().InvalidateMeasure();
        SamplesList().UpdateLayout();
        AppendLog(L"After first item layout: dictionaries = " + to_hstring(DictionaryCount()));

        // Populate the remaining rows after the cold first-use boundary, as the
        // task collection continues to receive items during normal operation.
        for (int i = 1; i < 500; ++i)
        {
            m_taskItems.Append(make<TaskSampleViewModel>(L"Task " + to_hstring(i)));
        }
        SamplesList().InvalidateMeasure();
        SamplesList().UpdateLayout();
        AppendLog(L"After adding tasks: dictionaries = " + to_hstring(DictionaryCount()));
        AppendLog(L"The XAML-fixed ItemTemplate is being realized by the visible ListView; scroll to realize later rows.");
    }

    void WETestWindow::RunColdList_Click(IInspectable const&, RoutedEventArgs const&)
    {
        RunList(false);
    }

    void WETestWindow::RunWarmList_Click(IInspectable const&, RoutedEventArgs const&)
    {
        RunList(true);
    }

    void WETestWindow::RunDirect_Click(IInspectable const&, RoutedEventArgs const&)
    {
        if (m_hasRun) return;
        m_hasRun = true;
        ControlChoice().IsEnabled(false);
        ColdListButton().IsEnabled(false);
        WarmListButton().IsEnabled(false);
        DirectButton().IsEnabled(false);

        AppendLog(SelectedControlName() + L" / direct creation");
        AppendLog(L"Before construction in Click: " + to_hstring(DictionaryCount()));
        auto control = CreateSelectedControl();
        AppendLog(L"After construction in Click: " + to_hstring(DictionaryCount()));
        DirectHost().Content(control);
    }

    void WETestWindow::SampleItem_Loaded(IInspectable const& sender, RoutedEventArgs const&)
    {
        ++m_loadedCount;
        if (m_loadedCount <= 3)
        {
            auto row = sender.try_as<FrameworkElement>();
            AppendLog(L"Row Loaded #" + to_hstring(m_loadedCount) + L": dictionaries = " + to_hstring(DictionaryCount()) + L", measured row height = " + to_hstring(row ? row.ActualHeight() : 0.0));
        }
    }

    void WETestWindow::TaskDataRow_Loaded(IInspectable const& sender, RoutedEventArgs const&)
    {
        // OpenNet synchronizes DataRow cell visibility when each virtualized
        // row is loaded, then requests a fresh measure/arrange pass.
        if (auto row = sender.try_as<winrt::WinUI3cppWorkTest::TaskDataRow>())
        {
            auto const implementation = winrt::get_self<TaskDataRow>(row);
            if (m_dataRowLoadedCount++ < 3)
            {
                AppendLog(L"TaskDataRow Loaded: children=" + to_hstring(row.Children().Size()) + L", MeasureOverride=" + to_hstring(implementation->MeasurePassCount()) + L", ArrangeOverride=" + to_hstring(implementation->ArrangePassCount()));
            }
            for (auto const& child : row.Children())
            {
                child.Visibility(Visibility::Visible);
            }
            row.InvalidateMeasure();
            row.InvalidateArrange();
        }
    }

    void WETestWindow::ProgressControl_Loaded(IInspectable const& sender, RoutedEventArgs const&)
    {
        if (auto progress = sender.try_as<WinUI3Package::ProgressBarEx>(); progress && m_progressLoadedCount++ < 4)
        {
            AppendLog(L"ProgressBarEx Loaded: dictionaries=" + to_hstring(DictionaryCount()) + L", size=" + to_hstring(progress.ActualWidth()) + L"x" + to_hstring(progress.ActualHeight()));
        }
    }
}
