#include "pch.h"
#include "XamlcTestPage.xaml.h"
#if __has_include("XamlcTestPage.g.cpp")
#include "XamlcTestPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WinUI3cppWorkTest::implementation
{
    int32_t XamlcTestPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void XamlcTestPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
