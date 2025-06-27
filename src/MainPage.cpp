#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "public.h"
#include <winrt\Windows.UI.Popups.h>

const vector<hstring> SupportedSchemas = { L"http", L"https", L"ftp", L"file" };

namespace winrt::SparsePackageRegisterUWP::implementation
{
    MainPage::MainPage()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        Dlg = local::Dialog();
        ChangeTitleBarTheme(*this, nullptr);
        MSIXFilePicker.SuggestedStartLocation(PickerLocationId::ComputerFolder); MSIXFilePicker.FileTypeFilter().Append(L".msix");
        Win32AppDirPicker.SuggestedStartLocation(PickerLocationId::ComputerFolder); Win32AppDirPicker.FileTypeFilter().Append(L"*");
        Dlg.ChangeStatus({ this, &MainPage::RegisterSprasePackage });
    }

    //Xaml binding properties and events

    //Properties
    hstring MainPage::Version()
    {
        PackageVersion Ver = Package::Current().Id().Version();
        return to_hstring(Ver.Major) + L"." + to_hstring(Ver.Minor) + L"." + to_hstring(Ver.Build) + L"." + to_hstring(Ver.Revision);
    }

    //Events
    void MainPage::LoadTheme(IInspectable const& sender, RoutedEventArgs const&)
    {
        int i = (unbox_value<int>(AppDC.Values().Lookup(L"AppTheme")) + 2) % 3;
        ChangeThemeText(i); sender.as<muxc::Expander>().Content().as<muxc::RadioButtons>().SelectedIndex(i);
     }

    void MainPage::AppThemeChanged(IInspectable const& sender, SelectionChangedEventArgs const&)
    {
        auto control = sender.as<muxc::RadioButtons>(); int AppTheme = control.SelectedIndex();
        Frame().RequestedTheme(static_cast<ElementTheme>((AppTheme + 1) % 3)); ChangeThemeText(AppTheme);
        AppDC.Values().Insert(L"AppTheme", box_value((AppTheme + 1) % 3));
    }

    void MainPage::ChangeTitleBarTheme(FrameworkElement const& sender, IInspectable const&)
    {
        if (sender.ActualTheme() == ElementTheme::Light)
        { TitleBar.ButtonForegroundColor(Colors::Black()); TitleBar.ButtonInactiveForegroundColor(Colors::DarkGray()); }
        else { TitleBar.ButtonForegroundColor(Colors::White()); TitleBar.ButtonInactiveForegroundColor(Colors::LightGray()); }
        Dlg.RequestedTheme(sender.ActualTheme());
    }

    fire_and_forget MainPage::OpenPicker(IInspectable const& sender, RoutedEventArgs const&)
    {
        Button Control = sender.as<Button>(); Control.IsEnabled(false);
        hstring option = unbox_value<hstring>(Control.Tag());
        if (option == L"MSIX") { GetFile(co_await MSIXFilePicker.PickSingleFileAsync()); }
        else if (option == L"Dir") { GetDir(co_await Win32AppDirPicker.PickSingleFolderAsync()); }
        Control.IsEnabled(true);
    }

    void MainPage::IdentifyObject(IInspectable const&, DragEventArgs const& e)
    {
        if (e.DataView().Contains(StandardDataFormats::StorageItems()))
        {
            IVectorView<IStorageItem> Items = e.DataView().GetStorageItemsAsync().get();
            if (Items.Size() == 1)
            {
                IStorageItem Item = Items.GetAt(0);
                if (!Item.IsOfType(StorageItemTypes::None))
                {
                    e.AcceptedOperation(DataPackageOperation::Link);
                    if (Item.IsOfType(StorageItemTypes::File))
                    { e.DragUIOverride().Caption(L"释放以选择文件"); }
                    else { e.DragUIOverride().Caption(L"释放以选择目录"); }
                }
            }
        }
    }

    void MainPage::GetIStorageItem(IInspectable const&, DragEventArgs const& e)
    {
        if (e.DataView().Contains(StandardDataFormats::StorageItems()))
        {
            IVectorView<IStorageItem> Items = e.DataView().GetStorageItemsAsync().get();
            if (Items.Size() == 1)
            {
                IStorageItem Item = Items.GetAt(0);
                if (Item.IsOfType(StorageItemTypes::File)) { GetFile(Item.as<StorageFile>()); }
                else if (Item.IsOfType(StorageItemTypes::Folder)) { GetDir(Item.as<StorageFolder>()); }
            }
        }
    }

    fire_and_forget MainPage::ShowWarning(IInspectable const&, RoutedEventArgs const&)
    {
        AllowUnsignedPackage().IsEnabled(false);
        Dlg.Glyph(L"\xE7BA"); Dlg.Text(L"警告！！！");
        if (co_await Dlg.ShowInf(L"注册未签名的包可能会给操作系统带来潜在风险，\n本程序作者不承担由此带来的一切后果！\n确定继续启用吗？",
            L"确定", L"取消", ContentDialogButton::Close) != ContentDialogResult::Primary)
        { AllowUnsignedPackage().IsChecked(false); }
        AllowUnsignedPackage().IsEnabled(true);
    }

    fire_and_forget MainPage::Register(IInspectable const&, RoutedEventArgs const&)
    {
        IsEnabled(false);
        Dlg.Glyph(L"\xE9D9"); Dlg.Text(L"正在注册");
        co_await Dlg.ShowStatus(ContentDialogButton::Close);
        IsEnabled(true);
    }

    //Private functions
    void MainPage::ChangeThemeText(int const& AppTheme)
    {
        hstring ThemeText = L"";
        switch (AppTheme)
        {
            case 0: ThemeText = L"浅色"; break;
            case 1: ThemeText = L"深色"; break;
            case 2: ThemeText = L"跟随系统"; break;
        } CurAppThemeDisplay().Text(ThemeText);
    }

    fire_and_forget MainPage::GetFile(StorageFile const& File)
    {
        if (File != nullptr)
        {
            if (File.Path() != L"") { MSIXPath().Text(File.Path()); }
            else
            {
                Dlg.Glyph(L"\xE783"); Dlg.Text(L"错误");
                co_await Dlg.ShowInf(L"不支持从 URL 获取文件，\n请重新选择！！！", L"", L"关闭", ContentDialogButton::Close);
            }
        }
    }

    fire_and_forget MainPage::GetDir(StorageFolder const& Dir)
    {
        if (Dir != nullptr)
        {
            if (Dir.Path() != L"") { Win32AppDirPath().Text(Dir.Path()); }
            else
            {
                Dlg.Glyph(L"\xE783"); Dlg.Text(L"错误");
                co_await Dlg.ShowInf(L"不支持此电脑、回收站等系统桌面图标（但支持用户文件夹），\n请重新选择！！！", L"", L"关闭", ContentDialogButton::Close);
            }
        }
    }

    fire_and_forget MainPage::RegisterSprasePackage(SparsePackageRegisterUWP::Dialog const&, IInspectable const&)
    {
        co_await resume_foreground(Dispatcher());
        Dlg.IsIndeterminate(true); Dlg.Status(L"正在检查注册参数，\n请耐心等待...");
        try
        {
            if (MSIXPath().Text() == L"")
            { Dlg.Show(L"Inf"); Dlg.Inf(L"MSIX 稀疏包路径不能为空！"); ShowErrorTitle(); }
            else if (Win32AppDirPath().Text() == L"")
            { Dlg.Show(L"Inf"); Dlg.Inf(L"Win32 应用目录路径不能为空！"); ShowErrorTitle(); }
            else
            {
                Uri MSIXUri{ MSIXPath().Text() };
                Uri Win32AppDirUri{ Win32AppDirPath().Text() };
                if (find(SupportedSchemas.begin(), SupportedSchemas.end(), MSIXUri.SchemeName()) == SupportedSchemas.end())
                { Dlg.Show(L"Inf"); Dlg.Inf(L"MSIX 稀疏包路径使用了不支持的协议：" + MSIXUri.SchemeName() + L"，应为以下协议：http，https，ftp，file。"); ShowErrorTitle(); }
                else if (Win32AppDirUri.SchemeName() != L"file")
                { Dlg.Show(L"Inf"); Dlg.Inf(L"Win32 应用目录路径必须指向本地！"); ShowErrorTitle(); }
                else
                {
                    Options.ExternalLocationUri(Win32AppDirUri);
                    Dlg.Status(L"注册任务即将开始，请耐心等待......");
                    Options.ForceAppShutdown(ForceClose().IsChecked().Value());
                    Options.ForceUpdateFromAnyVersion(ForceUpdate().IsChecked().Value());
                    Options.AllowUnsigned(AllowUnsignedPackage().IsChecked().Value());
                    auto AsyncTask = Manager.AddPackageByUriAsync(MSIXUri, Options);
                    if (AsyncTask == nullptr)
                    { throw hresult_error(E_FAIL, L"本程序已损坏，无法使用“直接管理其他应用”功能，\n因为未在本程序包清单中声明 <rescap:Capability Name=\"packageManagement\"/> 受限功能！"); }
                    AsyncTask.Progress({ this, &MainPage::UpdateProgress });
                    auto Result = co_await AsyncTask;
                    if (Result.IsRegistered())
                    {
                        Dlg.Glyph(L"\xE930"); Dlg.Text(L"注册成功");
                        Dlg.Show(L"Inf"); Dlg.Inf(L"MSIX 稀疏包注册成功！");
                    }
                    else
                    {
                        Dlg.Glyph(L"\xE783"); Dlg.Text(L"注册失败"); Dlg.Show(L"ErrorInf");
                        Dlg.ErrorHeader(L"MSIX 稀疏包注册失败，详细信息如下："); Dlg.ErrorInf(Result.ErrorText());
                    } AsyncTask.Progress(nullptr);
                }
            }
        }
        catch (hresult_error const& ex)
        {
            ShowErrorTitle(); Dlg.Show(L"ErrorInf");
            Dlg.ErrorHeader(L"注册时发生了错误，错误信息如下：");
            Dlg.ErrorInf(ex.message());
        } Dlg.CloseButtonText(L"关闭"); Dlg.Focus(FocusState::Programmatic);
    }

    fire_and_forget MainPage::UpdateProgress(DeploymentTaskHandler const&, DeploymentProgress const& Progress)
    {
        co_await resume_foreground(Dlg.Dispatcher());
        BOOL IsProcessing = Progress.state == DeploymentProgressState::Processing;
        if (IsProcessing) { Dlg.Progress(Progress.percentage); }
        Dlg.IsIndeterminate(false);
        Dlg.Status(L"正在" + hstring(IsProcessing ? L"" : L"等待") + L"注册 MSIX 稀疏包，\n请耐心等待......（已完成：" + to_hstring(Dlg.Progress()) + L" %）");
    }
}
