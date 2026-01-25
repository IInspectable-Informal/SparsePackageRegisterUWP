#pragma once
#include "Pages.RegisterPage.g.h"

namespace winrt::SparsePackageManager::Pages::implementation
{
    using DeploymentTaskHandler = Windows::Foundation::IAsyncOperationWithProgress<Windows::Management::Deployment::DeploymentResult, Windows::Management::Deployment::DeploymentProgress>;
    struct RegisterPage : RegisterPageT<RegisterPage>
    {
    public:
        RegisterPage();

        //Handlers
        fire_and_forget ShowWarning(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

        fire_and_forget OpenPicker(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

        void IdentifyObject(IInspectable const&, Windows::UI::Xaml::DragEventArgs const&);
        void GetIStorageItem(IInspectable const&, Windows::UI::Xaml::DragEventArgs const&);

        fire_and_forget Register(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

        //ITypeProvider
        Windows::UI::Xaml::Interop::TypeName Type();

    private:
        Windows::Storage::Pickers::FileOpenPicker MSIXFilePicker;
        Windows::Storage::Pickers::FolderPicker DesktopAppDirPicker;

        Windows::Management::Deployment::AddPackageOptions m_Options;

        local::Dialog m_Dialog = local::Dialog::Current();

        void GetFile(Windows::Storage::StorageFile const&);
        void GetDir(Windows::Storage::StorageFolder const&);
    };
}

namespace winrt::SparsePackageManager::Pages::factory_implementation
{
    struct RegisterPage : RegisterPageT<RegisterPage, implementation::RegisterPage>
    {
    };
}
