#include "pch.h"
#include "RootContainer.h"
#include "RootContainer.g.cpp"
#include "constants.h"
#include "helpers.h"
#include "TitleBarDefaultColors.h"

#include <winrt/Windows.UI.Popups.h>

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::UI;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
namespace muxc = Microsoft::UI::Xaml::Controls;

using namespace Windows::UI::Popups;

namespace winrt::SparsePackageManager::implementation
{
    IAsyncAction ShowMsgDialogAsync(hstring const& msg)
    {
        co_await MessageDialog(msg).ShowAsync();
    }

    RootContainer::RootContainer(Windows::UI::ViewManagement::ApplicationView const& view) : 
        m_ViewTitleBar(view.TitleBar()) ,
        m_Dialog(local::Dialog::Current())
    {
        InitializeComponent();
        TitleBar().Height(RootNavigationView().CompactPaneLength());
        auto items = RootNavigationView().MenuItems();
        items.GetAt(0).as<muxc::NavigationViewItem>().Tag(box_value(g_RegisterPageType));
        items.GetAt(1).as<muxc::NavigationViewItem>().Tag(box_value(g_PackageListPageType));

        IsPaneOnTop(AppDC.Values().Lookup(L"IsPaneOnTop").as<bool>());
    }

    //Handlers
    void RootContainer::ThemeChanged(FrameworkElement const& sender, IInspectable const&)
    {
        m_Dialog.RequestedTheme(sender.ActualTheme());
        if (sender.ActualTheme() == ElementTheme::Light)
        {
            m_ViewTitleBar.ButtonForegroundColor(Colors::Black());
            m_ViewTitleBar.ButtonInactiveForegroundColor(Colors::DarkGray());
        }
        else
        {
            m_ViewTitleBar.ButtonForegroundColor(Colors::White());
            m_ViewTitleBar.ButtonInactiveForegroundColor(Colors::LightGray());
        }
    }

    void RootContainer::OnTitleBarLoaded(IInspectable const& sender, RoutedEventArgs const&)
    {
        Window::Current().SetTitleBar(sender.as<UIElement>());

        if (AppDC.Values().HasKey(L"LastUsedPage"))
        {
            NavigateTo(AppDC.Values().Lookup(L"LastUsedPage").as<hstring>());
        }
        else
        {
            NavigateTo(g_RegisterPageType.Name);
        }
    }

    void RootContainer::NavViewDisplayModeChanged(muxc::NavigationView const& sender, muxc::NavigationViewDisplayModeChangedEventArgs const& e)
    {
        if (IsPaneOnTop())
        {
            LeftPlaceholder().Width(GridLengthHelper::FromPixels(0));
            sender.IsTitleBarAutoPaddingEnabled(true);
            TitleBar().Height(32);
        }
        else
        {
            LeftPlaceholder().Width(GridLengthHelper::FromPixels(
                e.DisplayMode() == muxc::NavigationViewDisplayMode::Minimal ? 88 : 48));
            sender.IsTitleBarAutoPaddingEnabled(false);
            TitleBar().Height(48);
        }
    }

    void RootContainer::NavigationRequested(muxc::NavigationView const&, muxc::NavigationViewItemInvokedEventArgs const& e)
    {
        auto container = e.InvokedItemContainer();
        if (!container.IsSelected())
        {
            if (e.IsSettingsInvoked())
            {
                rootFrame().Navigate(g_SettingsPageType, nullptr, e.RecommendedNavigationTransitionInfo());
            }
            else
            {
                auto page_class = container.as<muxc::NavigationViewItem>().Tag().as<TypeName>();
                rootFrame().Navigate(page_class, nullptr, e.RecommendedNavigationTransitionInfo());
            }
        }
    }

    void RootContainer::SelectedViewItemChanged(muxc::NavigationView const& sender, muxc::NavigationViewSelectionChangedEventArgs const& e)
    {
        auto item = e.SelectedItem().as<muxc::NavigationViewItem>();
        if (IsPaneOnTop() && e.IsSettingsSelected())
        {
            sender.Header(box_value(GetLocalizedStringFromPackageAndSubtree(
                GetWinUIDependencyPackage(), L"Microsoft.UI.Xaml/Resources", L"SettingsButtonName")));
        } else { sender.Header(item.Content()); }
        AppDC.Values().Insert(L"LastUsedPage", box_value(e.IsSettingsSelected() ?
            g_SettingsPageType.Name : item.Tag().as<TypeName>().Name));
    }

    void RootContainer::GoBackRequested(muxc::NavigationView const&, muxc::NavigationViewBackRequestedEventArgs const&)
    {
        if (rootFrame().CanGoBack())
        { rootFrame().GoBack(); }
    }

    void RootContainer::FrameNavigated(IInspectable const&, NavigationEventArgs const& e)
    {
        RootNavigationView().IsBackEnabled(rootFrame().CanGoBack());
        if (e.NavigationMode() == NavigationMode::Back)
        {
            auto page = rootFrame().Content();
            auto pageType = page.as<ITypeProvider>().Type();
            if (pageType == g_SettingsPageType)
            {
                RootNavigationView().SelectedItem(RootNavigationView().SettingsItem());
            }
            else
            {
                for (auto const& item : RootNavigationView().MenuItems())
                {
                    if (item.as<muxc::NavigationViewItem>().Tag().as<TypeName>() == pageType)
                    {
                        RootNavigationView().SelectedItem(item);
                        break;
                    }
                }
            }
        }
    }

    //Private functions
    void RootContainer::NavigateTo(hstring const& typeName)
    {
        if (typeName == g_SettingsPageType.Name)
        {
            rootFrame().Navigate(g_SettingsPageType);
            RootNavigationView().SelectedItem(RootNavigationView().SettingsItem());
        }
        else
        {
            for (auto const& item : RootNavigationView().MenuItems())
            {
                auto pageType = item.as<muxc::NavigationViewItem>().Tag().as<TypeName>();
                if (pageType.Name == typeName)
                {
                    rootFrame().Navigate(pageType);
                    RootNavigationView().SelectedItem(item);
                    break;
                }
            }
        }
    }

    //Properties
    bool RootContainer::IsPaneOnTop()
    { return RootNavigationView().PaneDisplayMode() == muxc::NavigationViewPaneDisplayMode::Top; }

    void RootContainer::IsPaneOnTop(bool const& value)
    {
        RootNavigationView().PaneDisplayMode(value ?
            muxc::NavigationViewPaneDisplayMode::Top : muxc::NavigationViewPaneDisplayMode::Auto);
        AppDC.Values().Insert(L"IsPaneOnTop", box_value(value));
    }

    //Static Properties
    winrt::slim_mutex RootContainer::sm_mutex;
    RootContainerMap RootContainer::sm_instances;

    local::RootContainer RootContainer::Current()
    {
        winrt::slim_lock_guard lock(sm_mutex);
        auto view = ApplicationView::GetForCurrentView();
        int viewId = view.Id();
            
        auto it = sm_instances.find(viewId);
        if (it != sm_instances.end())
        {
            if (auto instance = it->second.get())
            { return instance; }
            sm_instances.erase(it);
        }
            
        auto newInstance = winrt::make<implementation::RootContainer>(view);
        sm_instances.emplace(viewId, newInstance);
        return newInstance;
    }

    hstring RootContainer::AppName()
    { return thisAppInfo.DisplayInfo().DisplayName(); }

    Uri RootContainer::AppIcon()
    { return thisPackage.Logo(); }

    hstring RootContainer::Developer()
    { return thisPackage.PublisherDisplayName(); }

    hstring RootContainer::Version()
    {
        PackageVersion ver = thisPackage.Id().Version();
        return to_hstring(ver.Major) + L"." +
               to_hstring(ver.Minor) + L"." +
               to_hstring(ver.Build) + L"." +
               to_hstring(ver.Revision);
    }
}
