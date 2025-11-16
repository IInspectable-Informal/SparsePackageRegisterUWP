#pragma once
#include "Pages.PackageListPage.g.h"

namespace winrt::SparsePackageManager::Pages::implementation
{
    struct PackageListPage : PackageListPageT<PackageListPage>
    {
    public:
        PackageListPage();

        //Handlers
        fire_and_forget RefreshPackageListRequested(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        fire_and_forget LaunchIStorageItem(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

        //ITypeProvider
        Windows::UI::Xaml::Interop::TypeName Type();

    private:
        Windows::Foundation::Collections::IObservableVector<locald::PackageInfo> m_PackageInfoVec = single_threaded_observable_vector<locald::PackageInfo>();
        const Windows::System::LauncherOptions options;

        static const Windows::UI::Xaml::Interop::TypeName sm_Type;
    };
}

namespace winrt::SparsePackageManager::Pages::factory_implementation
{
    struct PackageListPage : PackageListPageT<PackageListPage, implementation::PackageListPage>
    {
    };
}
