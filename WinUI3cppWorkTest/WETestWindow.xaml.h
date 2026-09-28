#pragma once

#include "WETestWindow.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow>
    {
        WETestWindow()
        {
            ExtendsContentIntoTitleBar(true);
        }


        void ScrollView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };

    struct Contact
    {
        Contact(winrt::hstring firstName, winrt::hstring lastName, winrt::hstring company)
            : m_firstName(std::move(firstName))
            , m_lastName(std::move(lastName))
            , m_company(std::move(company))
        {
        }

        winrt::hstring FirstName() const
        {
            return m_firstName;
        }
        winrt::hstring LastName() const
        {
            return m_lastName;
        }
        winrt::hstring Company() const
        {
            return m_company;
        }
        winrt::hstring Name() const
        {
            return m_firstName + L" " + m_lastName;
        }

    private:
        winrt::hstring m_firstName;
        winrt::hstring m_lastName;
        winrt::hstring m_company;
    };

}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow, implementation::WETestWindow>
    {
    };
}
