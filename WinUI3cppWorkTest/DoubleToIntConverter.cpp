#include "pch.h"
#include "DoubleToIntConverter.h"
#if __has_include("DoubleToIntConverter.g.cpp")
#include "DoubleToIntConverter.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::WinUI3cppWorkTest::implementation
{
    winrt::Windows::Foundation::IInspectable DoubleToIntConverter::Convert(
        winrt::Windows::Foundation::IInspectable const& value,
        [[maybe_unused]] winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
        winrt::Windows::Foundation::IInspectable const& parameter,
        [[maybe_unused]] winrt::hstring const& language)
    {
        //auto boolValue = winrt::unbox_value<bool>(value);
        //if (Reverse() || (parameter && winrt::unbox_value<winrt::hstring>(parameter) == L"Reverse"))
        //    boolValue = !boolValue;

        //return winrt::box_value(Convert::BoolToVisibility(boolValue));
        return winrt::box_value(static_cast<int32_t>(winrt::unbox_value<double>(value)));
    }

    winrt::Windows::Foundation::IInspectable DoubleToIntConverter::ConvertBack(
        [[maybe_unused]] winrt::Windows::Foundation::IInspectable const& value,
        [[maybe_unused]] winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
        [[maybe_unused]] winrt::Windows::Foundation::IInspectable const& parameter,
        [[maybe_unused]] winrt::hstring const& language)
    {
        throw std::exception{ "Not implemented" };
    }
    // <svg:SvgImageSource BindSizeTo="{x:Bind ImageTest}" / >
}
