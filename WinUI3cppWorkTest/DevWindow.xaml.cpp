#include "pch.h"
#include "DevWindow.xaml.h"
#if __has_include("DevWindow.g.cpp")
#include "DevWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WinUI3cppWorkTest::implementation
{
	DevWindow::DevWindow()
	{
		InitializeComponent();
	}

	bool DevWindow::BindTestButton()
	{
		return false;
	}

	double DevWindow::TestDouble()
	{
		return 22.0;
	}
	void DevWindow::TestDouble(double value)
	{
		throw hresult_not_implemented();
	}

	void DevWindow::Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{

	}

	void DevWindow::MenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{
		OutputDebugString((L"MenuFlyoutItem_Click\n" + winrt::unbox_value<winrt::hstring>(e.OriginalSource())).c_str());
	}


}
