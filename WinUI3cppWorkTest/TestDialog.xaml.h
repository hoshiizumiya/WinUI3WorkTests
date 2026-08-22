#pragma once

#include "TestDialog.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct TestDialog : TestDialogT<TestDialog>
    {
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct TestDialog : TestDialogT<TestDialog, implementation::TestDialog>
    {
    };
}
