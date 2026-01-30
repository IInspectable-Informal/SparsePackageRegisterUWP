#pragma once
#include "Pages.PackageListPage.g.h"

namespace winrt::SparsePackageManager::Pages::implementation
{
    struct PackageListPage : PackageListPageT<PackageListPage>
    {
    public:
        PackageListPage();

        //Handlers
        fire_and_forget RefreshPackageListRequested(Microsoft::UI::Xaml::Controls::RefreshContainer const&, Microsoft::UI::Xaml::Controls::RefreshRequestedEventArgs const&);
        void RefreshByClickRequested(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        fire_and_forget LaunchIStorageItemRequested(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        fire_and_forget RemovePackageRequested(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        fire_and_forget LaunchIStorageItemRequested2(Microsoft::UI::Xaml::Controls::SwipeItem const&, Microsoft::UI::Xaml::Controls::SwipeItemInvokedEventArgs const&);
        fire_and_forget RemovePackageRequested2(Microsoft::UI::Xaml::Controls::SwipeItem const&, Microsoft::UI::Xaml::Controls::SwipeItemInvokedEventArgs const&);

        //ITypeProvider
        Windows::UI::Xaml::Interop::TypeName Type();

    private:
        Windows::Foundation::Collections::IObservableVector<locald::PackageInfo> m_PackageInfoVec = single_threaded_observable_vector<locald::PackageInfo>();
        const Windows::System::LauncherOptions options;

        local::Dialog m_Dialog;

        Windows::Foundation::IAsyncAction LaunchIStorageItemRequestedImplAsync(IInspectable const&);
        Windows::Foundation::IAsyncAction RemovePackageRequestedImplAsync(hstring const&);
    };
}

namespace winrt::SparsePackageManager::Pages::factory_implementation
{
    struct PackageListPage : PackageListPageT<PackageListPage, implementation::PackageListPage>
    {
    };
}
