#include "pch.h"
#include "Pages/PackageListPage.h"
#include "Pages.PackageListPage.g.cpp"
#include "constants.h"
#include "helpers.hpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::System;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
namespace muxc = Microsoft::UI::Xaml::Controls;

namespace winrt::SparsePackageManager::Pages::implementation
{
    PackageListPage::PackageListPage() :
        m_Dialog(local::Dialog::Current())
    {
        InitializeComponent();
        options.DisplayApplicationPicker(true);
        PkgListView().ItemsSource(m_PackageInfoVec);
    }

    //Handlers
    fire_and_forget PackageListPage::RefreshPackageListRequested(muxc::RefreshContainer const&, muxc::RefreshRequestedEventArgs const& e)
    {
        auto deferral = e.GetDeferral();
        RefreshButton().IsEnabled(false);
        PackagesCountReport().Text(GetLocalizedString(L"EnumeratingText"));
        m_PackageInfoVec.Clear();
        hresult_error ex{0};
        try
        {
            for (auto const& pkg : GetPackageManagerForCurrentThread()->FindPackagesForUser(L""))
            {
                try
                {
                if (!pkg.EffectiveExternalPath().empty())
                {
                    m_PackageInfoVec.Append(co_await locald::PackageInfo::CreateInstanceAsync(pkg));
                }
                }
                catch (hresult_error const& ex0)
                {
                    auto file = ApplicationData::Current().LocalFolder().CreateFileAsync(L"log2.log", CreationCollisionOption::OpenIfExists).get();
                    FileIO::AppendTextAsync(file, ex0.message() + L"\n").get();
                }
            }
            PackagesCountReport().Text(
                runtime_format(GetLocalizedString(L"PackagesCountReportText"), m_PackageInfoVec.Size())
            );
        }
        catch (hresult_error const& ex0)
        { ex = ex0; }
        if (ex.code() != 0)
        {
            PackagesCountReport().Text(GetLocalizedString(L"ErrorOnEnumeratingText"));
            co_await m_Dialog.ShowErrorAsync(
                (ex.code() == 0x80070005 ?
                    GetLocalizedString(L"PackageTamperedText2") :
                    std::format(L"{}\nHRESULT: 0x{:08X}", ex.message(), ex.code()).c_str()
                ), GetLocalizedString(L"ErrorHeaderText")
            );
        }
        deferral.Complete();
        deferral.Close();
        RefreshButton().IsEnabled(true);
    }

    void PackageListPage::RefreshByClickRequested(IInspectable const&, RoutedEventArgs const&)
    { PullToRefresh().RequestRefresh(); }

    fire_and_forget PackageListPage::LaunchIStorageItemRequested(IInspectable const& sender, RoutedEventArgs const&)
    {
        auto control = sender.as<MenuFlyoutItem>();
        control.IsEnabled(false);
        auto obj = control.Tag();
        auto iids = get_interfaces(obj);
        if (std::find(iids.begin(), iids.end(), guid_of<IStorageFile>()) != iids.end())
        { co_await Launcher::LaunchFileAsync(obj.as<StorageFile>(), options); }
        else { co_await Launcher::LaunchFolderPathAsync(obj.as<hstring>()); }
        control.IsEnabled(true);
    }

    fire_and_forget PackageListPage::RemovePackageRequested(IInspectable const& sender, RoutedEventArgs const&)
    {
        auto pkgFullName = sender.as<FrameworkElement>().Tag().as<hstring>();
        auto item = locald::TaskInfo::CreateInstance(pkgFullName);
        if (item)
        {
            GetTaskInfoListForCurrentThread().InsertAt(0, item);
            if (co_await item.RunTaskAsync())
            { PullToRefresh().RequestRefresh(); }
            else { local::RootContainer::Current().Navigate(g_TaskListPageType); }
        }
        else
        {
            co_await m_Dialog.ShowErrorAsync(
                GetLocalizedString(L"PackageTamperedText"),
                GetLocalizedString(L"ErrorHeaderText")
            );
        }
    }

    //ITypeProvider
    TypeName PackageListPage::Type()
    { return g_PackageListPageType; }
}
