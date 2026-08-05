#pragma once

#include "DoubleToIntConverter.g.h"

namespace winrt::WinUI3cppWorkTest::implementation
{
    struct DoubleToIntConverter : DoubleToIntConverterT<DoubleToIntConverter>
    {
        winrt::Windows::Foundation::IInspectable Convert(
            winrt::Windows::Foundation::IInspectable const& value,
            winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
            winrt::Windows::Foundation::IInspectable const& parameter,
            winrt::hstring const& language
        );

        winrt::Windows::Foundation::IInspectable ConvertBack(
            winrt::Windows::Foundation::IInspectable const& value,
            winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
            winrt::Windows::Foundation::IInspectable const& parameter,
            winrt::hstring const& language
        );
    };
}

namespace winrt::WinUI3cppWorkTest::factory_implementation
{
    struct DoubleToIntConverter : DoubleToIntConverterT<DoubleToIntConverter, implementation::DoubleToIntConverter>
    {
    };
}
