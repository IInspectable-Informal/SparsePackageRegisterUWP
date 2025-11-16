#include "pch.h"
#include "App.h"
#include "constants.h"

#include <winrt/Windows.UI.Popups.h>

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::System::Profile;
using namespace Windows::Storage;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

using namespace Windows::UI::Popups;

namespace winrt::SparsePackageManager::implementation
{
    /// <summary>
    /// Creates the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        if (wcstoull(AnalyticsInfo::VersionInfo().DeviceFamilyVersion().c_str(), nullptr, 10) < 0x000A00004A610000)
        { throw hresult_not_implemented(L"最低操作系统版本低于 10.0.19041.0，无法启动应用程序！"); }
        Suspending({ this, &App::OnSuspending });
        if (!AppDC.Values().HasKey(L"AppTheme"))
        { AppDC.Values().Insert(L"AppTheme", box_value(0)); }
        if (!AppDC.Values().HasKey(L"IsPaneOnTop"))
        { AppDC.Values().Insert(L"IsPaneOnTop", box_value(false)); }
        #if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
        #endif
        UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e) -> IAsyncAction
        {
            e.Handled(true);
            auto file = co_await ApplicationData::Current().LocalFolder().CreateFileAsync(L"log.log", CreationCollisionOption::OpenIfExists);
            co_await FileIO::AppendTextAsync(file, e.Message() + L"\n");
        });
    }

    /// <summary>
    /// Invoked when the application is launched normally by the end user.  Other entry points
    /// will be used such as when the application is launched to open a specific file.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        CreateView();
    }

    /// <summary>
    /// Invoked when application execution is being suspended.  Application state is saved
    /// without knowing whether the application will be terminated or resumed with the contents
    /// of memory still intact.
    /// </summary>
    /// <param name="sender">The source of the suspend request.</param>
    /// <param name="e">Details about the suspend request.</param>
    void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
    {
        // Save application state and stop any background activity
    }

    fire_and_forget App::CreateView()
    {
        try {
        if (!Inited) { Inited = true; InitView(); }
        else
        {
            co_await resume_foreground(CoreApplication::CreateNewView().Dispatcher());
            int id = InitView();
            co_await ApplicationViewSwitcher::TryShowAsStandaloneAsync(id);
        }
        }
        catch (hresult_error const& ex)
        {
            auto file = ApplicationData::Current().LocalFolder().CreateFileAsync(L"log0.log", CreationCollisionOption::OpenIfExists).get();
            FileIO::AppendTextAsync(file, ex.message() + L"\n").get();
        }
    }

    int App::InitView()
    {
        CoreApplication::GetCurrentView().TitleBar().ExtendViewIntoTitleBar(true);
        Window curWin = Window::Current();
        ApplicationView appView = ApplicationView::GetForCurrentView();
        ApplicationViewTitleBar titleBar = appView.TitleBar();
        titleBar.ButtonBackgroundColor(Colors::Transparent());
        titleBar.ButtonInactiveBackgroundColor(Colors::Transparent());
        auto rootControl = local::RootContainer::Current();
        rootControl.RequestedTheme(static_cast<ElementTheme>(unbox_value<int>(AppDC.Values().Lookup(L"AppTheme"))));
        curWin.Content(rootControl);
        curWin.Activate();
        return appView.Id();
    }
}
