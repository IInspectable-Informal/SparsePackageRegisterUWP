#include "pch.h"
#include "Pages/SettingsPage.h"
#include "Pages.SettingsPage.g.cpp"
#include "constants.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::SparsePackageManager::Pages::implementation
{
    SettingsPage::SettingsPage()
    {
        InitializeComponent();
    }

    //Handlers
    void SettingsPage::ThemeSelectionLoaded(IInspectable const& sender, RoutedEventArgs const&)
    {
        int i = (unbox_value<int>(AppDC.Values().Lookup(L"AppTheme")) + 2) % 3;
        sender.as<ComboBox>().SelectedIndex(i);
    }

    void SettingsPage::AppThemeChanged(IInspectable const& sender, SelectionChangedEventArgs const&)
    {
        auto control = sender.as<ComboBox>(); int selection = control.SelectedIndex();
        if (selection != -1)
        {
            int themeNum = (selection + 1) % 3;
            local::RootContainer::Current().RequestedTheme(static_cast<ElementTheme>(themeNum));
            AppDC.Values().Insert(L"AppTheme", box_value(themeNum));
        } else { ThemeSelectionLoaded(sender, nullptr); }
    }

    void SettingsPage::NavStyleLoaded(IInspectable const& sender, RoutedEventArgs const&)
    { sender.as<ComboBox>().SelectedIndex(AppDC.Values().Lookup(L"IsPaneOnTop").as<bool>()); }

    void SettingsPage::NavStyleChanged(IInspectable const& sender, SelectionChangedEventArgs const&)
    {
        auto control = sender.as<ComboBox>(); int selection = control.SelectedIndex();
        if (selection != -1)
        { local::RootContainer::Current().IsPaneOnTop(selection); }
        else { NavStyleLoaded(sender, nullptr); }
    }

    //ITypeProvider
    TypeName SettingsPage::Type()
    { return g_SettingsPageType; }
}
