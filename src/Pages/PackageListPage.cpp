#include "pch.h"
#include "Pages/PackageListPage.h"
#include "Pages.PackageListPage.g.cpp"
#include "helpers.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::System;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::SparsePackageManager::Pages::implementation
{
    PackageListPage::PackageListPage()
    {
        InitializeComponent();
        options.DisplayApplicationPicker(true);
        PkgListView().ItemsSource(m_PackageInfoVec);
    }

    //Handlers
    fire_and_forget PackageListPage::RefreshPackageListRequested(IInspectable const&, RoutedEventArgs const&)
    {
        RefreshButton().IsEnabled(false);
        m_PackageInfoVec.Clear();
        for (auto const& pkg : GetPackageManagerForCurrentThread()->FindPackagesForUser(L""))
        {
            try
            {
            if (!pkg.EffectiveExternalPath().empty())
            {
                m_PackageInfoVec.Append(co_await locald::PackageInfo::CreateInstanceAsync(pkg));
            }
            }
            catch (hresult_error const& ex)
            {
                auto file = ApplicationData::Current().LocalFolder().CreateFileAsync(L"log2.log", CreationCollisionOption::OpenIfExists).get();
                FileIO::AppendTextAsync(file, ex.message() + L"\n").get();
            }
        }
        RefreshButton().IsEnabled(true);
    }

    fire_and_forget PackageListPage::LaunchIStorageItem(IInspectable const& sender, RoutedEventArgs const&)
    {
        auto control = sender.as<MenuFlyoutItem>();
        control.IsEnabled(false);
        auto fsObject = control.Tag().as<IStorageItem>();
        if (fsObject.IsOfType(StorageItemTypes::File))
        { co_await Launcher::LaunchFileAsync(fsObject.as<StorageFile>(), options); }
        else { co_await Launcher::LaunchFolderAsync(fsObject.as<StorageFolder>()); }
        control.IsEnabled(true);
    }

    //ITypeProvider
    TypeName PackageListPage::Type()
    { return PackageListPage::sm_Type; }

    const TypeName PackageListPage::sm_Type = xaml_typename<localp::PackageListPage>();
}
