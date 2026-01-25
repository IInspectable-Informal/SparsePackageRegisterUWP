#include "pch.h"
#include "Controls/SettingItem.h"
#include "Controls.SettingItem.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace winrt::SparsePackageManager::Controls::implementation
{
    SettingItem::SettingItem()
    {
        DefaultStyleKey(box_value(SettingItem::s_Type));
    }

    //Properties
    IconElement SettingItem::Icon()
    { return GetValue(SettingItem::s_IconProperty).as<IconElement>(); }

    void SettingItem::Icon(IconElement const& value)
    { SetValue(SettingItem::s_IconProperty, value); }

    IInspectable SettingItem::Header()
    { return GetValue(SettingItem::s_HeaderProperty); }

    void SettingItem::Header(IInspectable const& value)
    { SetValue(SettingItem::s_HeaderProperty, value); }

    //FrameworkElement
    void SettingItem::OnApplyTemplate()
    {
        //box_value(*this).as<IFrameworkElementOverrides>().OnApplyTemplate();
    }

    //Static Properties
    DependencyProperty SettingItem::IconProperty()
    { return SettingItem::s_IconProperty; }

    DependencyProperty SettingItem::HeaderProperty()
    { return SettingItem::s_HeaderProperty; }

    const ::Interop::TypeName SettingItem::s_Type = xaml_typename<localc::SettingItem>();

    const DependencyProperty SettingItem::s_IconProperty = DependencyProperty::Register(
       L"Icon",
       xaml_typename<IconElement>(),
       SettingItem::s_Type,
       PropertyMetadata(nullptr));

    const DependencyProperty SettingItem::s_HeaderProperty = DependencyProperty::Register(
       L"Header",
       xaml_typename<IInspectable>(),
       SettingItem::s_Type,
       PropertyMetadata(nullptr));
}
