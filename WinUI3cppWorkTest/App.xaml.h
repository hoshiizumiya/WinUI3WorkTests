#pragma once

#include "App.xaml.g.h"
#include "DoubleToIntConverter.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
