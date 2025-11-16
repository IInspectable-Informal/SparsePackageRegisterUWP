#pragma once

namespace winrt
{
    hstring GetLocalizedString(hstring const&);
    hstring GetLocalizedStringFromPackageAndSubtree(Windows::ApplicationModel::Package const&, hstring const&, hstring const&);

    Windows::ApplicationModel::Package GetWinUIDependencyPackage();

    std::shared_ptr<Windows::Management::Deployment::PackageManager> GetPackageManagerForCurrentThread();
}