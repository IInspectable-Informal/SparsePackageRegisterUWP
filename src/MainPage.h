#pragma once
#include "pch.h"
#include "MainPage.g.h"

//WinRT
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Windows.Management.Deployment.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Management::Deployment;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
namespace muxc = Microsoft::UI::Xaml::Controls;

using namespace std;
typedef IAsyncOperationWithProgress<DeploymentResult, DeploymentProgress> DeploymentTaskHandler;

namespace winrt::SprasePackageRegisterUWP::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        public:
            MainPage();

            //XAML binding properties and events
            //Properties
            hstring AppName() { return AppInfo::Current().DisplayInfo().DisplayName(); }
            Uri AppIcon() { return Package::Current().Logo(); }
            hstring Developer() { return Package::Current().PublisherDisplayName(); }
            hstring Version();

            //Events
            void LoadTheme(IInspectable const&, RoutedEventArgs const&);
            void AppThemeChanged(IInspectable const&, SelectionChangedEventArgs const&);
            void ChangeTitleBarTheme(FrameworkElement const&, IInspectable const&);
            fire_and_forget OpenPicker(IInspectable const&, RoutedEventArgs const&);
            void IdentifyObject(IInspectable const&, DragEventArgs const&);
            void GetIStorageItem(IInspectable const&, DragEventArgs const&);
            fire_and_forget ShowWarning(IInspectable const&, RoutedEventArgs const&);
            fire_and_forget Register(IInspectable const&, RoutedEventArgs const&);
            void SetFocus(IInspectable const& sender, DependencyPropertyChangedEventArgs const&)
            { if (IsEnabled()) { sender.as<Button>().Focus(FocusState::Programmatic); } }
        private:
            ApplicationViewTitleBar TitleBar = ApplicationView::GetForCurrentView().TitleBar();
            void ChangeThemeText(int const&);
            FileOpenPicker MSIXFilePicker; FolderPicker Win32AppDirPicker;
            fire_and_forget GetFile(StorageFile const&);
            fire_and_forget GetDir(StorageFolder const&);
            local::Dialog Dlg = nullptr;
            PackageManager Manager; AddPackageOptions Options;
            fire_and_forget RegisterSprasePackage(SprasePackageRegisterUWP::Dialog const&, IInspectable const&);
            fire_and_forget UpdateProgress(DeploymentTaskHandler const&, DeploymentProgress const&);
            void ShowErrorTitle() { Dlg.Glyph(L"\xE783"); Dlg.Text(L"错误"); }
    };
}

namespace winrt::SprasePackageRegisterUWP::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
