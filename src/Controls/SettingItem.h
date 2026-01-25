#pragma once
#include "Controls.SettingItem.g.h"

namespace winrt::SparsePackageManager::Controls::implementation
{
    struct SettingItem : SettingItemT<SettingItem>
    {
    public:
        SettingItem();

        //Properties
        Windows::UI::Xaml::Controls::IconElement Icon();
        void Icon(Windows::UI::Xaml::Controls::IconElement const&);
        IInspectable Header();
        void Header(IInspectable const&);

        //FrameworkElement
        void OnApplyTemplate();

        //Static Properties
        static Windows::UI::Xaml::DependencyProperty IconProperty();
        static Windows::UI::Xaml::DependencyProperty HeaderProperty();

    private:
        static const Windows::UI::Xaml::Interop::TypeName s_Type;

        static const Windows::UI::Xaml::DependencyProperty s_IconProperty;
        static const Windows::UI::Xaml::DependencyProperty s_HeaderProperty;
    };
}

namespace winrt::SparsePackageManager::Controls::factory_implementation
{
    struct SettingItem : SettingItemT<SettingItem, implementation::SettingItem>
    {
    };
}
