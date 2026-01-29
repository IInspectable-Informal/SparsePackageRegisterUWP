#include "pch.h"
#include "Data/PackageInfo.h"
#include "Data.PackageInfo.g.cpp"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media;

namespace winrt::SparsePackageManager::Data::implementation
{
    PackageInfo::PackageInfo(Package const& pkg, StorageFile const& manifest) :
        m_DisplayName(pkg.DisplayName()),
        m_Developer(pkg.PublisherDisplayName()),
        m_InstalledDate(pkg.InstalledDate()),
        m_ExternalPath(pkg.EffectiveExternalPath()),
        m_PackageFullName(pkg.Id().FullName()),
        m_UserExternalPath(pkg.UserExternalPath()),
        m_MachineExternalPath(pkg.MachineExternalPath()),
        m_Manifest(manifest)
    {
        
    }

    hstring PackageInfo::DisplayName()
    { return m_DisplayName; }

    hstring PackageInfo::Developer()
    { return m_Developer; }

    DateTime PackageInfo::InstalledDate()
    { return m_InstalledDate; }

    hstring PackageInfo::ExternalPath()
    { return m_ExternalPath; }

    hstring PackageInfo::PackageFullName()
    { return m_PackageFullName; }

    hstring PackageInfo::UserExternalPath()
    { return m_UserExternalPath; }

    hstring PackageInfo::MachineExternalPath()
    { return m_MachineExternalPath; }

    ImageSource PackageInfo::Logo()
    { return m_Logo; }

    StorageFile PackageInfo::Manifest()
    { return m_Manifest; }

    //Methods
    hstring PackageInfo::GetDeveloperAndInstalledDateCombination()
    {
        DateTimeFormatter formatter(L"shortdate");
        return m_Developer + L" | " + formatter.Format(m_InstalledDate);
    }

    //Protected Methods
    IAsyncAction PackageInfo::SetSourceAsync(IRandomAccessStream const& logoStream)
    { co_await m_Logo.SetSourceAsync(logoStream); }

    //Static Methods
    constexpr Size g_LogoSize{ 64, 64 };
    IAsyncOperation<locald::PackageInfo> PackageInfo::CreateInstanceAsync(Package const& package)
    {
        auto manifest = co_await package.InstalledLocation().GetFileAsync(L"AppxManifest.xml");
        auto instance = make_self<implementation::PackageInfo>(package, manifest);
        try
        {
            auto logoStream = co_await package.GetLogoAsRandomAccessStreamReference(g_LogoSize).OpenReadAsync();
            co_await instance->SetSourceAsync(logoStream);
        }
        catch (hresult_error const& ex)
        {
            auto file = ApplicationData::Current().LocalFolder().CreateFileAsync(L"log1.log", CreationCollisionOption::OpenIfExists).get();
            FileIO::AppendTextAsync(file, ex.message() + L"\n").get();
        }
        co_return instance.as<locald::PackageInfo>();
    }
}
