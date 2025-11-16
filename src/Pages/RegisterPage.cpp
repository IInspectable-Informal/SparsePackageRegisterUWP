#include "pch.h"
#include "Pages/RegisterPage.h"
#include "Pages.RegisterPage.g.cpp"
#include "helpers.h"

using namespace winrt;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::SparsePackageManager::Pages::implementation
{
    RegisterPage::RegisterPage()
    {
        InitializeComponent();
        MSIXFilePicker.SuggestedStartLocation(PickerLocationId::ComputerFolder);
        MSIXFilePicker.FileTypeFilter().ReplaceAll({ L".msix", L"*" });
        DesktopAppDirPicker.SuggestedStartLocation(PickerLocationId::ComputerFolder);
        DesktopAppDirPicker.FileTypeFilter().Append(L"*");
    }

    //Handlers
    fire_and_forget RegisterPage::ShowWarning(IInspectable const&, RoutedEventArgs const&)
    {
        auto dialog = local::Dialog::Current();
        AllowUnsignedPackage().IsEnabled(false);
        dialog.Glyph(L"\xE7BA"); dialog.Text(GetLocalizedString(L"WarningTitle"));
        if (co_await dialog.ShowInfoAsync(GetLocalizedString(L"WarningContent"),
            GetLocalizedString(L"ContentDialogConfirm"), GetLocalizedString(L"ContentDialogCancel"),
            ContentDialogButton::Close) != ContentDialogResult::Primary)
        { AllowUnsignedPackage().IsChecked(false); }
        AllowUnsignedPackage().IsEnabled(true);
    }

    fire_and_forget RegisterPage::OpenPicker(IInspectable const& sender, RoutedEventArgs const&)
    {
        auto control = sender.as<Button>(); control.IsEnabled(false);
        hstring option = unbox_value<hstring>(control.Tag());
        if (option == L"MSIX") { GetFile(co_await MSIXFilePicker.PickSingleFileAsync()); }
        else if (option == L"Dir") { GetDir(co_await DesktopAppDirPicker.PickSingleFolderAsync()); }
        control.IsEnabled(true);
    }

    void RegisterPage::IdentifyObject(IInspectable const&, DragEventArgs const& e)
    {
        if (e.DataView().Contains(StandardDataFormats::StorageItems()))
        {
            auto items = e.DataView().GetStorageItemsAsync().get();
            if (items.Size() == 1)
            {
                IStorageItem item = items.GetAt(0);
                if (!item.IsOfType(StorageItemTypes::None))
                {
                    e.AcceptedOperation(DataPackageOperation::Link);
                    if (item.IsOfType(StorageItemTypes::File))
                    { e.DragUIOverride().Caption(GetLocalizedString(L"ReleaseToSelectFile")); }
                    else { e.DragUIOverride().Caption(GetLocalizedString(L"ReleaseToSelectDir")); }
                }
            }
        }
    }

    void RegisterPage::GetIStorageItem(IInspectable const&, DragEventArgs const& e)
    {
        if (e.DataView().Contains(StandardDataFormats::StorageItems()))
        {
            auto items = e.DataView().GetStorageItemsAsync().get();
            if (items.Size() == 1)
            {
                IStorageItem item = items.GetAt(0);
                if (item.IsOfType(StorageItemTypes::File)) { GetFile(item.as<StorageFile>()); }
                else if (item.IsOfType(StorageItemTypes::Folder)) { GetDir(item.as<StorageFolder>()); }
            }
        }
    }

    //Private functions
    void RegisterPage::GetFile(StorageFile const& file)
    {
        if (file != nullptr)
        {
            if (file.Path() != L"")
            { MSIXPath().Text(file.Path()); }
        }
    }

    void RegisterPage::GetDir(StorageFolder const& dir)
    {
        if (dir != nullptr)
        {
            if (dir.Path() != L"") 
            { DesktopAppDirPath().Text(dir.Path()); }
        }
    }

    //ITypeProvider
    TypeName RegisterPage::Type()
    { return RegisterPage::sm_Type; }

    const TypeName RegisterPage::sm_Type = xaml_typename<localp::RegisterPage>();
}
