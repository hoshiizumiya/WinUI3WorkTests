#include "pch.h"
#include "WETestWindow.xaml.h"
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
        // Prepare the bound task collection before XAML connects ItemsSource.
        // The list remains Collapsed until a test starts, so row templates are
        // realized by XAML only after the scenario has installed its template.
        m_taskItems = single_threaded_observable_vector<IInspectable>();
        for (int i = 0; i < 500; ++i)
        {
            m_taskItems.Append(box_value(L"Task " + to_hstring(i)));
        }
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

    DataTemplate WETestWindow::SelectedTemplate()
    {
        hstring key;
        switch (ControlChoice().SelectedIndex())
        {
        case 0: key = L"ProgressSample"; break;
        case 1: key = L"CardSample"; break;
        case 2: key = L"ExpanderSample"; break;
        case 3: key = L"GroupSample"; break;
        default: throw hresult_invalid_argument(L"Choose a control first.");
        }
        return TestRoot().Resources().Lookup(box_value(key)).as<DataTemplate>();
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

        auto const control = SelectedControlName();
        auto const before = DictionaryCount();
        AppendLog(control + (warm ? L" / prewarm then XAML layout" : L" / cold XAML layout"));
        AppendLog(L"Before first construction: " + to_hstring(before));

        if (warm)
        {
            // Load this control's global resources before XAML realizes rows.
            auto prewarmed = CreateSelectedControl();
            AppendLog(L"After construction in Click: " + to_hstring(DictionaryCount()));
        }

        // ItemsSource is x:Bind-connected before startup layout. Installing the
        // row template and revealing the populated, virtualized ListView causes
        // XAML itself to measure the list and realize the visible task rows.
        SamplesList().ItemTemplate(SelectedTemplate());
        SamplesList().Visibility(Visibility::Visible);
        SamplesList().InvalidateMeasure();
        AppendLog(L"List revealed; XAML is measuring and realizing visible rows.");
        AppendLog(L"Scroll to realize later rows. Only visible rows should load initially.");
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

    void WETestWindow::SampleItem_Loaded(IInspectable const&, RoutedEventArgs const&)
    {
        ++m_loadedCount;
        if (m_loadedCount <= 3)
        {
            AppendLog(L"Row Loaded #" + to_hstring(m_loadedCount) + L": dictionaries = " + to_hstring(DictionaryCount()));
        }
    }
}
