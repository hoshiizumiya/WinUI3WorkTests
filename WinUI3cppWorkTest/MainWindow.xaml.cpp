#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <Windows.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Networking;
using namespace Windows::Networking::Connectivity;
using namespace Microsoft::UI::Windowing;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WinUI3cppWorkTest::implementation
{
	MainWindow::MainWindow()
	{
		InitializeComponent();
		//WebView1().EnsureCoreWebView2Async();
		//do_work();
		//IP().Text(MainWindow::GetLocalIPv6Addresses()[0]);
		// 防止越界：没有 IPv6 时不要直接取 [0]
		auto ips = MainWindow::GetLocalIPv6Addresses();
		if (!ips.empty())
		{
			IP().Text(ips[0]);
		}
	}

	std::vector<std::wstring> MainWindow::GetLocalIPv6Addresses()
	{
		std::vector<std::wstring> out;
		auto hostnames = NetworkInformation::GetHostNames();
		for (auto const& hn : hostnames)
		{
			if (hn.Type() == HostNameType::Ipv6 && hn.IPInformation())
			{
				auto ip = hn.CanonicalName(); // e.g. "fe80::1%15" 或 "240e:..."
				// 过滤回环、链路本地（按需）
				bool isLoopback = ip == L"::1";
				//bool isLinkLocal = ip.size() >= 5 && ip.substr(0, 5) == L"fe80:";
				if (!isLoopback)
				{
					out.push_back(std::wstring(ip));
				}
			}
		}
		return out;
	}

	void MainWindow::Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{
		ContentDialog dlg{};
		dlg.XamlRoot(this->Content().as<FrameworkElement>().XamlRoot());
		dlg.Title(box_value(L"标题"));
		dlg.Content(box_value(L"内容"));
		dlg.PrimaryButtonText(L"确定");
		dlg.CloseButtonText(L"取消");
		dlg.DefaultButton(ContentDialogButton::Primary);

		dlg.ShowAsync();

	}

	void MainWindow::Button2_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{
		using namespace Microsoft::UI::Xaml;
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Windowing;
		using Windows::Graphics::RectInt32;
		// 父窗口
		Window owner = *this;

		// 1) 新建次级窗口（将要显示为“对话框风格”的独立窗口）
		Window dialog{};

		// 2) 组装一点简单的内容
		auto root = StackPanel();
		root.Spacing(12);
		root.Padding(Thickness{ 160, 160, 160, 160 });

		auto title = TextBlock();
		title.Text(L"自定义对话框窗口");
		title.FontSize(20);

		auto msg = TextBlock();
		msg.Text(L"这是使用 AppWindow + OverlappedPresenter::CreateForDialog() 创建的次级窗口。");

		auto buttons = StackPanel();
		buttons.Orientation(Orientation::Horizontal);
		buttons.Spacing(8);

		auto okBtn = Button();
		okBtn.Content(box_value(L"确定"));

		auto cancelBtn = Button();
		cancelBtn.Content(box_value(L"取消"));

		buttons.Children().Append(okBtn);
		buttons.Children().Append(cancelBtn);

		root.Children().Append(title);
		root.Children().Append(msg);
		root.Children().Append(buttons);

		dialog.Content(root);

		// 3) 设置对话框风格的 Presenter（注意：这不等同于“模态”）
		auto presenter = OverlappedPresenter::CreateForDialog();
		dialog.AppWindow().SetPresenter(presenter);
		dialog.AppWindow().IsShownInSwitchers(false); // 不在 Alt+Tab 里显示

		// 4) 实现“应用内模态”：禁用父窗口的根内容，关闭时恢复
		auto ownerRoot = owner.Content().as<UIElement>();
		//ownerRoot.IsEnabled(false);

		// 关闭逻辑（带返回值的地方你可以记录结果）
		auto closeWith = [owner, ownerRoot, dialog](bool /*accepted*/)
		{
			//ownerRoot.IsEnabled(true);
			owner.Activate(); // 把焦点还给父窗口
			dialog.Close();
			// TODO: 在这里处理 accepted（确定/取消）的结果
		};

		okBtn.Click([=](IInspectable const&, RoutedEventArgs const&)
		{
			closeWith(true);
		});
		cancelBtn.Click([=](IInspectable const&, RoutedEventArgs const&)
		{
			closeWith(false);
		});

		// 用户用 Alt+F4 或标题栏关闭按钮关闭时，确保恢复父窗口
		dialog.Closed([=](auto&&, auto&&)
		{
			//ownerRoot.IsEnabled(true);this func may be deprecated
			owner.Activate();
		});

		// 5) 尺寸与居中到父窗口
		const int width = 2120;
		const int height = 1220;

		auto ownerPos = owner.AppWindow().Position();
		auto ownerSize = owner.AppWindow().Size();

		RectInt32 target{};
		target.X = ownerPos.X + (ownerSize.Width - width) / 2;
		target.Y = ownerPos.Y + (ownerSize.Height - height) / 2;
		target.Width = width;
		target.Height = height;

		dialog.AppWindow().MoveAndResize(target);

		// 6) 显示
		dialog.Activate();
	}

	void MainWindow::BackdropCombo_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
	{

	}

	void MainWindow::TextBlock_PointerEntered(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));

	}

	void MainWindow::MenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{

	}

}
