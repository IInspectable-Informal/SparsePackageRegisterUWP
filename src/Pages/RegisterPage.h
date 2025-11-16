#pragma once
#include "Pages.RegisterPage.g.h"

namespace winrt::SparsePackageManager::Pages::implementation
{
    struct RegisterPage : RegisterPageT<RegisterPage>
    {
    public:
        RegisterPage();

        //Handlers
        fire_and_forget ShowWarning(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

        fire_and_forget OpenPicker(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

        void IdentifyObject(IInspectable const&, Windows::UI::Xaml::DragEventArgs const&);
        void GetIStorageItem(IInspectable const&, Windows::UI::Xaml::DragEventArgs const&);

        //ITypeProvider
        Windows::UI::Xaml::Interop::TypeName Type();

    private:
        Windows::Storage::Pickers::FileOpenPicker MSIXFilePicker;
        Windows::Storage::Pickers::FolderPicker DesktopAppDirPicker;

        void GetFile(Windows::Storage::StorageFile const&);
        void GetDir(Windows::Storage::StorageFolder const&);

        static const Windows::UI::Xaml::Interop::TypeName sm_Type;
    };
}

namespace winrt::SparsePackageManager::Pages::factory_implementation
{
    struct RegisterPage : RegisterPageT<RegisterPage, implementation::RegisterPage>
    {
    };
}
