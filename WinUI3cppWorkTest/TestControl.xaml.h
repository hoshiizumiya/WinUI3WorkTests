#pragma once

#include "TestControl.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct TestControl : TestControlT<TestControl>
    {



    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct TestControl : TestControlT<TestControl, implementation::TestControl>
    {
    };
}
