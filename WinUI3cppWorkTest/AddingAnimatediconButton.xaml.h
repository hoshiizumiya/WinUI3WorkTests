#pragma once

#include "AddingAnimatediconButton.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct AddingAnimatediconButton : AddingAnimatediconButtonT<AddingAnimatediconButton>
    {
        AddingAnimatediconButton()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        void Button_PointerEntered(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);

        void Button_PointerExited(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);

    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct AddingAnimatediconButton : AddingAnimatediconButtonT<AddingAnimatediconButton, implementation::AddingAnimatediconButton>
    {
    };
}
