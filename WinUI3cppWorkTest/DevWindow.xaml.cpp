#include "pch.h"
#include "DevWindow.xaml.h"
#if __has_include("DevWindow.g.cpp")
#include "DevWindow.g.cpp"
#endif
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
using namespace winrt;

using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage::Streams;

using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Controls::Primitives;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Xaml::Media::Imaging;

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
        // 如果这是绑定属性，这里应保存 value，
        // 而不是直接抛出 hresult_not_implemented。
        OutputDebugString(
            std::format(L"TestDouble changed to {}\n", value).c_str());
    }

    void DevWindow::Button_Click(
        IInspectable const& sender,
        RoutedEventArgs const& args)
    {
    }

    void DevWindow::MenuFlyoutItem_Click(
        IInspectable const& sender,
        RoutedEventArgs const& args)
    {
        // OriginalSource 返回 IInspectable，不是 hstring，
        // 因此不能直接 unbox_value<hstring>()。
        if (auto element = args.OriginalSource().try_as<FrameworkElement>())
        {
            std::wstring message = L"MenuFlyoutItem_Click: ";
            message += element.Name();
            message += L"\n";

            OutputDebugString(message.c_str());
        }
        else
        {
            OutputDebugString(L"MenuFlyoutItem_Click\n");
        }
    }

    bool DevWindow::IsChecked(ToggleButton const& button)
    {
        if (!button)
        {
            return false;
        }

        IReference<bool> const checked = button.IsChecked();

        return checked && checked.Value();
    }
    fire_and_forget DevWindow::SourceGrid_DragStarting(
        UIElement const&,
        DragStartingEventArgs const& args)
    {
        auto lifetime = get_strong();

        args.Data().SetText(SourceTextBox().Text());

        if (IsChecked(DataPackageRB()))
        {
            args.DragUI().SetContentFromDataPackage();
            co_return;
        }

        if (!IsChecked(CustomContentRB()))
        {
            co_return;
        }

        auto deferral = args.GetDeferral();

        try
        {
            RenderTargetBitmap renderTargetBitmap;

            co_await renderTargetBitmap.RenderAsync(
                SourceTextBox());

            IBuffer const pixels =
                co_await renderTargetBitmap.GetPixelsAsync();

            SoftwareBitmap const bitmap =
                SoftwareBitmap::CreateCopyFromBuffer(
                    pixels,
                    BitmapPixelFormat::Bgra8,
                    renderTargetBitmap.PixelWidth(),
                    renderTargetBitmap.PixelHeight());

            args.DragUI().SetContentFromSoftwareBitmap(bitmap);
        }
        catch (hresult_error const& error)
        {
            OutputDebugStringW(error.message().c_str());
        }

        deferral.Complete();
    }

    void DevWindow::TargetTextBox_DragEnter(
        IInspectable const&,
        DragEventArgs const& args)
    {
        VisualStateManager::GoToState(
            RootControl(),
            L"Inside",
            true);

        DataPackageView const dataView = args.DataView();

        bool const hasText =
            dataView.Contains(StandardDataFormats::Text());

        args.AcceptedOperation(
            hasText
            ? DataPackageOperation::Copy
            : DataPackageOperation::None);

        if (!hasText)
        {
            return;
        }

        auto dragUI = args.DragUIOverride();
        dragUI.Caption(L"Drop here");

        if (IsChecked(HideRB()))
        {
            dragUI.IsGlyphVisible(false);
            dragUI.IsContentVisible(false);
        }
    }

    void DevWindow::TargetTextBox_DragLeave(
        IInspectable const&,
        DragEventArgs const&)
    {
        VisualStateManager::GoToState(
            RootControl(),
            L"Outside",
            true);
    }

    fire_and_forget DevWindow::TargetTextBox_Drop(
        IInspectable const&,
        DragEventArgs const& args)
    {
        auto lifetime = get_strong();
        auto deferral = args.GetDeferral();

        VisualStateManager::GoToState(
            RootControl(),
            L"Outside",
            true);

        DataPackageView const dataView = args.DataView();

        bool const hasText =
            dataView.Contains(StandardDataFormats::Text());

        args.AcceptedOperation(
            hasText
            ? DataPackageOperation::Copy
            : DataPackageOperation::None);

        if (hasText)
        {
            try
            {
                hstring const droppedText =
                    co_await dataView.GetTextAsync();

                std::wstring result{
                    TargetTextBox().Text()
                };

                result += droppedText;

                TargetTextBox().Text(result);
            }
            catch (hresult_error const& error)
            {
                OutputDebugStringW(error.message().c_str());
            }
        }

        deferral.Complete();
    }
}
