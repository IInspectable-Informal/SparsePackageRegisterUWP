#pragma once
#include "App.xaml.g.h"

namespace winrt::SparsePackageManager::implementation
{
    struct App : AppT<App>
    {
        public:
            App();

            void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
            void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        private:
            bool Inited = false;

            fire_and_forget CreateView();
            int InitView();
    };
}

// ============================================================================
// XAML TYPEINFO FIXUP - WORKAROUND FOR CPP/WINRT TOOLCHAIN BUG
// ============================================================================
// 
// ⚠️  DO NOT MODIFY OR REMOVE THIS SECTION
//
// Problem:
//   XAML compiler's type generator incorrectly resolves nested namespaces
//   for runtimeclass pages, causing compilation failures in XamlTypeInfo.g.cpp
//
// Symptoms:
//   Error C2039: "implementation" is not a member of Pages namespace
//   Error C3083: "implementation" symbol must be a type
//
// Effect of removal:
//   XamlTypeInfo.g.cpp will fail to compile with namespace resolution errors
//   for implementation and factory_implementation classes
//
// Maintenance:
//   Update includes when adding new pages to the project
//   Monitor Microsoft toolchain updates for permanent fix availability
// ============================================================================
#include "Pages/PackageListPage.h"
#include "Pages/RegisterPage.h"
#include "Pages/SettingsPage.h"
#if __has_include("Pages.PackageListPage.xaml.g.hpp")
#include "Pages.PackageListPage.xaml.g.hpp"
#endif
#if __has_include("Pages.RegisterPage.xaml.g.hpp")
#include "Pages.RegisterPage.xaml.g.hpp"
#endif
#if __has_include("Pages.SettingsPage.xaml.g.hpp")
#include "Pages.SettingsPage.xaml.g.hpp"
#endif
