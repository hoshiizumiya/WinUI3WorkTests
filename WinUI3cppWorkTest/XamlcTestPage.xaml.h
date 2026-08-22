#pragma once

#include "XamlcTestPage.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct XamlcTestPage : XamlcTestPageT<XamlcTestPage>
    {
        XamlcTestPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct XamlcTestPage : XamlcTestPageT<XamlcTestPage, implementation::XamlcTestPage>
    {
    };
}
