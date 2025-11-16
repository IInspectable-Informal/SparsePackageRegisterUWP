#pragma once
#include "Pages.SettingsPage.g.h"

namespace winrt::SparsePackageManager::Pages::implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage>
    {
    public:
        SettingsPage();

        //Handlers
        void ThemeSelectionLoaded(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        void AppThemeChanged(IInspectable const&, Windows::UI::Xaml::Controls::SelectionChangedEventArgs const&);
        void NavStyleLoaded(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        void NavStyleChanged(IInspectable const&, Windows::UI::Xaml::Controls::SelectionChangedEventArgs const&);

        //ITypeProvider
        Windows::UI::Xaml::Interop::TypeName Type();

    private:
        static const Windows::UI::Xaml::Interop::TypeName sm_Type;
    };
}

namespace winrt::SparsePackageManager::Pages::factory_implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage, implementation::SettingsPage>
    {
    };
}
