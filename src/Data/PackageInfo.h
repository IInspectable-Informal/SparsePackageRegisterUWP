#pragma once
#include "Data.PackageInfo.g.h"

namespace winrt::SparsePackageManager::Data::implementation
{
    struct PackageInfo : PackageInfoT<PackageInfo>
    {
    public:
        PackageInfo(Windows::ApplicationModel::Package const&, Windows::Storage::StorageFile const&);

        hstring DisplayName();
        hstring Developer();
        Windows::Foundation::DateTime InstalledDate();
        hstring ExternalPath();
        hstring PackageFullName();
        hstring UserExternalPath();
        hstring MachineExternalPath();
        Windows::UI::Xaml::Media::ImageSource Logo();
        Windows::Storage::StorageFile Manifest();

        //Methods
        hstring GetDeveloperAndInstalledDateCombination();

        //Static Methods
        static Windows::Foundation::IAsyncOperation<locald::PackageInfo> CreateInstanceAsync(Windows::ApplicationModel::Package const&);

    protected:
        Windows::Foundation::IAsyncAction SetSourceAsync(Windows::Storage::Streams::IRandomAccessStream const&);

    private:
        const hstring m_DisplayName;
        const hstring m_Developer;
        const Windows::Foundation::DateTime m_InstalledDate;
        const hstring m_ExternalPath;
        const hstring m_PackageFullName;
        const hstring m_UserExternalPath;
        const hstring m_MachineExternalPath;
        const Windows::UI::Xaml::Media::Imaging::BitmapImage m_Logo;
        const Windows::Storage::StorageFile m_Manifest;
    };
}

namespace winrt::SparsePackageManager::Data::factory_implementation
{
    struct PackageInfo : PackageInfoT<PackageInfo, implementation::PackageInfo>
    {
    };
}
