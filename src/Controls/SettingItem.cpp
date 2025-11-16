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
        DefaultStyleKey(box_value(SettingItem::sm_SettingItemType));
    }

    //Properties
    IconElement SettingItem::Icon()
    { return GetValue(SettingItem::sm_IconProperty).as<IconElement>(); }

    void SettingItem::Icon(IconElement const& value)
    { SetValue(SettingItem::sm_IconProperty, value); }

    IInspectable SettingItem::Header()
    { return GetValue(SettingItem::sm_HeaderProperty); }

    void SettingItem::Header(IInspectable const& value)
    { SetValue(SettingItem::sm_HeaderProperty, value); }

    //FrameworkElement
    void SettingItem::OnApplyTemplate()
    {
        //box_value(*this).as<IFrameworkElementOverrides>().OnApplyTemplate();
    }

    //Static Properties
    DependencyProperty SettingItem::IconProperty()
    { return SettingItem::sm_IconProperty; }

    DependencyProperty SettingItem::HeaderProperty()
    { return SettingItem::sm_HeaderProperty; }

    const ::Interop::TypeName SettingItem::sm_SettingItemType = xaml_typename<localc::SettingItem>();

    const DependencyProperty SettingItem::sm_IconProperty = DependencyProperty::Register(
       L"Icon",
       xaml_typename<IconElement>(),
       SettingItem::sm_SettingItemType,
       PropertyMetadata(nullptr));

    const DependencyProperty SettingItem::sm_HeaderProperty = DependencyProperty::Register(
       L"Header",
       xaml_typename<IInspectable>(),
       SettingItem::sm_SettingItemType,
       PropertyMetadata(nullptr));
}
