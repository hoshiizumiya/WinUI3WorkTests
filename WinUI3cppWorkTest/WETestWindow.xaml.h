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


    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct WETestWindow : WETestWindowT<WETestWindow, implementation::WETestWindow>
    {
    };
}
