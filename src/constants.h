#pragma once

namespace winrt
{
    const Windows::ApplicationModel::Package thisPackage = Windows::ApplicationModel::Package::Current();
    const Windows::ApplicationModel::AppInfo thisAppInfo = Windows::ApplicationModel::AppInfo::Current();

    const Windows::Storage::ApplicationDataContainer AppDC = Windows::Storage::ApplicationData::Current().RoamingSettings();

    const Windows::ApplicationModel::Resources::Core::ResourceContext g_ResCtx =
        Windows::ApplicationModel::Resources::Core::ResourceContext::GetForViewIndependentUse();

    const Windows::UI::Xaml::Interop::TypeName g_TaskListPageType = xaml_typename<localp::TaskListPage>();
    const Windows::UI::Xaml::Interop::TypeName g_RegisterPageType = xaml_typename<localp::RegisterPage>();
    const Windows::UI::Xaml::Interop::TypeName g_PackageListPageType = xaml_typename<localp::PackageListPage>();
    const Windows::UI::Xaml::Interop::TypeName g_SettingsPageType = xaml_typename<localp::SettingsPage>();
}