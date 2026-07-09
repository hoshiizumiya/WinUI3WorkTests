#include "pch.h"
#include "AddingAnimatediconButton.xaml.h"
#if __has_include("AddingAnimatediconButton.g.cpp")
#include "AddingAnimatediconButton.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WinUI3cppWorkTest::implementation
{
    void AddingAnimatediconButton::Button_PointerEntered(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        // static method
        AnimatedIcon::SetState(SearchAnimatedIcon(), L"PointerOver");
    }

    void AddingAnimatediconButton::Button_PointerExited(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        AnimatedIcon::SetState(SearchAnimatedIcon(), L"Normal");
    }
}
