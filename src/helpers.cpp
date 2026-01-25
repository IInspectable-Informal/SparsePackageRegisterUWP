#include "pch.h"
#include "constants.h"
#include "helpers.h"

//Standard Library
#include <mutex>

//Win32
#include <processthreadsapi.h>

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::ApplicationModel::Resources::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Management::Deployment;
using namespace Windows::UI::ViewManagement;

namespace winrt
{
    ResourceLoader g_ThisPkgRes = nullptr;

    hstring GetLocalizedString(hstring const& resName)
    {
        if (!g_ThisPkgRes)
        { g_ThisPkgRes = ResourceLoader::GetForCurrentView(); }
        return g_ThisPkgRes.GetString(resName);
    }

    using SubtreeMap = std::unordered_map<hstring, ResourceMap>;
    using PkgResMap = std::unordered_map<hstring, SubtreeMap>;

    PkgResMap g_ResMap;

    const IMapView<hstring, ResourceMap> g_AllResMap = ResourceManager::Current().AllResourceMaps();

    hstring GetLocalizedStringFromPackageAndSubtree(Package const& pkg, hstring const& subtree, hstring const& resName)
    {
        if (!pkg) { return L""; }

        const auto& pkgName = pkg.Id().Name();
        if (!g_AllResMap.HasKey(pkgName)) { return L""; }

        const auto& pkgGlobalRes = g_AllResMap.Lookup(pkgName);

        auto pkgIt = g_ResMap.find(pkgName);
        if (pkgIt == g_ResMap.end())
        { pkgIt = g_ResMap.try_emplace(pkgName).first; }

        ResourceMap subMap = nullptr;
        if (subtree.empty())
        { subMap = pkgGlobalRes; }
        else
        {
            auto& subtreeMap = pkgIt->second;
            auto subtreeIt = subtreeMap.find(subtree);
            if (subtreeIt == subtreeMap.end())
            {
                subMap = pkgGlobalRes.GetSubtree(subtree);
                if (!subMap) { return L""; }
                subtreeIt = subtreeMap.try_emplace(subtree, subMap).first;
            } else { subMap = subtreeIt->second; }
        }

        if (resName.empty())
        { return L""; }
        return subMap.GetValue(resName, g_ResCtx).ValueAsString();
    }

    const std::wstring g_WinUIDependencyPkgPartName = L"Microsoft.UI.Xaml";
    Package g_WinUIDependencyPkg = nullptr;
    Package GetWinUIDependencyPackage()
    {
        if (!g_WinUIDependencyPkg)
        {
            for (auto const& dependency : thisPackage.Dependencies())
            {
                if (std::wstring(dependency.Id().Name().c_str()).find(g_WinUIDependencyPkgPartName) != std::wstring::npos)
                {
                    g_WinUIDependencyPkg = dependency;
                    break;
                }
            }
        } return g_WinUIDependencyPkg;
    }

    std::map<DWORD, std::shared_ptr<PackageManager>> g_PMMap;
    std::mutex g_PMMutex;

    std::shared_ptr<PackageManager> GetPackageManagerForCurrentThread()
    {
        DWORD threadId = GetCurrentThreadId();

        std::lock_guard<std::mutex> lock(g_PMMutex);
        auto iterator = g_PMMap.find(threadId);
        if (iterator == g_PMMap.end())
        {
            PackageManager pm;
            iterator = g_PMMap.try_emplace(threadId, std::make_shared<PackageManager>(std::move(pm))).first;
        } return iterator->second;
    }

    std::mutex g_TILMMutex;
    std::map<int, TaskInfoList> g_TaskInfoListMap;
    TaskInfoList GetTaskInfoListForCurrentThread()
    {
        int id = ApplicationView::GetForCurrentView().Id();
        std::lock_guard<std::mutex> lock(g_TILMMutex);
        auto it = g_TaskInfoListMap.find(id);
        if (it == g_TaskInfoListMap.end())
        {
            it = g_TaskInfoListMap.try_emplace(id, single_threaded_observable_vector<locald::TaskInfo>()).first;
        } return it->second;
    }
}
