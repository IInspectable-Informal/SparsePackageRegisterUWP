#include "pch.h"
#include "Pages/RegisterPage.h"
#include "Pages.RegisterPage.g.cpp"
#include "constants.h"
#include "helpers.hpp"

using namespace winrt;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation;
using namespace Windows::Management::Deployment;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

const std::vector<hstring> SupportedSchemas = { L"http", L"https", L"ftp", L"file" };

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
        AllowUnsignedPackage().IsEnabled(false);
        m_Dialog.Glyph(L"\xE7BA"); m_Dialog.Text(GetLocalizedString(L"WarningTitle"));
        if (co_await m_Dialog.ShowInfoAsync(GetLocalizedString(L"WarningContent"),
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

    fire_and_forget RegisterPage::Register(IInspectable const&, RoutedEventArgs const&)
    {
        IsEnabled(false);
        auto filePath = MSIXPath().Text();
        auto dirPath = DesktopAppDirPath().Text();
        if (filePath == L"")
        { co_await m_Dialog.ShowErrorAsync(GetLocalizedString(L"PathErrorText1")); }
        else if (dirPath == L"")
        { co_await m_Dialog.ShowErrorAsync(GetLocalizedString(L"PathErrorText2")); }
        else
        {
            hresult_error ex{ 0 };
            try
            {
                Uri fileUri{ filePath }; Uri dirUri{ dirPath };
                if (find(SupportedSchemas.begin(), SupportedSchemas.end(), fileUri.SchemeName()) == SupportedSchemas.end())
                { co_await m_Dialog.ShowErrorAsync(runtime_format(GetLocalizedString(L"UriErrorText1"), fileUri.SchemeName().c_str())); }
                else if (dirUri.SchemeName() != L"file")
                { co_await m_Dialog.ShowErrorAsync(GetLocalizedString(L"UriErrorText2")); }
                else
                {
                    m_Options.ExternalLocationUri(dirUri);
                    m_Options.ForceAppShutdown(ForceClose().IsChecked().Value());
                    m_Options.ForceUpdateFromAnyVersion(ForceUpdate().IsChecked().Value());
                    m_Options.AllowUnsigned(AllowUnsignedPackage().IsChecked().Value());
                    auto item = locald::TaskInfo::CreateInstance(fileUri, m_Options);
                    if (item)
                    {
                        GetTaskInfoListForCurrentThread().InsertAt(0, item);
                        IsEnabled(true);
                        if (co_await item.RunTaskAsync())
                        { co_return; }
                        else { local::RootContainer::Current().Navigate(g_TaskListPageType); }
                        co_return;
                    }
                    else
                    {
                        co_await m_Dialog.ShowErrorAsync(
                            GetLocalizedString(L"PackageTamperedText1"),
                            GetLocalizedString(L"ErrorHeaderText")
                        );
                    }
                }
            }
            catch (hresult_error const& ex0)
            { ex = ex0; }
            if (ex.code() != 0)
            {
                co_await m_Dialog.ShowErrorAsync(
                    std::format(L"{}\nHRESULT: 0x{:08X}", ex.message(), ex.code()).c_str(),
                    GetLocalizedString(L"ErrorHeaderText")
                );
            }
        } IsEnabled(true);
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
    { return g_RegisterPageType; }
}
