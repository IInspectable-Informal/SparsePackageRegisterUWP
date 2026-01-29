#pragma once

namespace winrt
{
    using TaskInfoList = Windows::Foundation::Collections::IObservableVector<locald::TaskInfo>;

    hstring GetLocalizedString(hstring const&);
    hstring GetLocalizedStringFromPackageAndSubtree(Windows::ApplicationModel::Package const&, hstring const&, hstring const&);

    Windows::ApplicationModel::Package GetWinUIDependencyPackage();

    std::shared_ptr<Windows::Management::Deployment::PackageManager> GetPackageManagerForCurrentThread();

    TaskInfoList GetTaskInfoListForCurrentThread();

    void SendToast(hstring const&, hstring const&);
}