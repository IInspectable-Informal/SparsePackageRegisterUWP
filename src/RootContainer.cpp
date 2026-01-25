#include "pch.h"
#include "RootContainer.h"
#include "RootContainer.g.cpp"
#include "constants.h"
#include "helpers.h"
#include "TitleBarDefaultColors.h"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::UI;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::Xaml::Navigation;
namespace muxc = Microsoft::UI::Xaml::Controls;

namespace winrt::SparsePackageManager::implementation
{
    RootContainer::RootContainer(Windows::UI::ViewManagement::ApplicationView const& view) : 
        m_ViewTitleBar(view.TitleBar()) ,
        m_Dialog(local::Dialog::Current()) ,
        m_TypeList({ g_TaskListPageType , g_RegisterPageType, g_PackageListPageType })
    {
        InitializeComponent();
        TitleBar().Height(RootNavigationView().CompactPaneLength());
        auto items = RootNavigationView().MenuItems();
        uint32_t size = items.Size();
        for (uint32_t i = 0; i < size; ++i)
        { items.GetAt(i).as<FrameworkElement>().Tag(box_value(m_TypeList.at(i))); }

        IsPaneOnTop(AppDC.Values().Lookup(L"IsPaneOnTop").as<bool>());
    }

    //Handlers
    void RootContainer::ThemeChanged(FrameworkElement const& sender, IInspectable const&)
    {
        auto theme = sender.ActualTheme();
        m_Dialog.RequestedTheme(theme);
        if (theme == ElementTheme::Light)
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

        auto values = AppDC.Values();
        if (values.HasKey(L"LastUsedPage") && values.HasKey(L"PageTypeKind"))
        {
            TypeName type
            {
                values.Lookup(L"LastUsedPage").as<hstring>(),
                static_cast<TypeKind>(values.Lookup(L"PageTypeKind").as<int>())
            }; Navigate(type);
        }
        else
        {
            Navigate(g_TaskListPageType);
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
            TypeName pageType{0};
            if (e.IsSettingsInvoked())
            { pageType = g_SettingsPageType; }
            else { pageType = container.as<FrameworkElement>().Tag().as<TypeName>(); }
            rootFrame().Navigate(pageType, nullptr, e.RecommendedNavigationTransitionInfo());
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
    }

    void RootContainer::GoBackRequested(muxc::NavigationView const&, muxc::NavigationViewBackRequestedEventArgs const&)
    {
        if (rootFrame().CanGoBack())
        { rootFrame().GoBack(); }
    }

    void RootContainer::FrameNavigated(IInspectable const&, NavigationEventArgs const& e)
    {
        RootNavigationView().IsBackEnabled(rootFrame().CanGoBack());
        auto page = rootFrame().Content();
        auto pageType = page.as<ITypeProvider>().Type();
        if (e.NavigationMode() == NavigationMode::Back)
        {
            auto view = RootNavigationView();
            if (pageType == g_SettingsPageType)
            {
                view.SelectedItem(RootNavigationView().SettingsItem());
            }
            else
            {
                auto currentit = std::find(m_TypeList.begin(), m_TypeList.end(), pageType);
                uint32_t currentPos = static_cast<uint32_t>(std::distance(m_TypeList.begin(), currentit));
                view.SelectedItem(view.MenuItems().GetAt(currentPos));
            }
        }
        auto values = AppDC.Values();
        values.Insert(L"LastUsedPage", box_value(pageType.Name));
        values.Insert(L"PageTypeKind", box_value(static_cast<int>(pageType.Kind)));
    }

    //INavigate
    bool RootContainer::Navigate(TypeName const& pageType)
    {
        auto page = rootFrame().Content();
        auto view = RootNavigationView();
        if (page && IsPaneOnTop())
        {
            auto info = SlideNavigationTransitionInfo();
            if (pageType == g_SettingsPageType)
            {
                info.Effect(SlideNavigationTransitionEffect::FromRight);
                view.SelectedItem(view.SettingsItem());
            }
            else
            {
                auto current = page.as<ITypeProvider>().Type();
                auto currentit = std::find(m_TypeList.begin(), m_TypeList.end(), current);
                auto nextit = std::find(m_TypeList.begin(), m_TypeList.end(), pageType);
                uint64_t currentPos = std::distance(m_TypeList.begin(), currentit);
                uint64_t nextPos = std::distance(m_TypeList.begin(), nextit);
                auto cmp = nextPos <=> currentPos;
                if (cmp < 0) // nextPos < currentPos
                { info.Effect(SlideNavigationTransitionEffect::FromLeft); }
                else if (cmp > 0) // nextPos > currentPos
                { info.Effect(SlideNavigationTransitionEffect::FromRight); }
                else { return false; }
                view.SelectedItem(view.MenuItems().GetAt(static_cast<uint32_t>(nextPos)));
            } return rootFrame().Navigate(pageType, nullptr, info);
        }
        else
        {
            if (pageType == g_SettingsPageType)
            { view.SelectedItem(view.SettingsItem()); }
            else
            {
                auto nextit = std::find(m_TypeList.begin(), m_TypeList.end(), pageType);
                uint32_t nextPos = static_cast<uint32_t>(std::distance(m_TypeList.begin(), nextit));
                view.SelectedItem(view.MenuItems().GetAt(nextPos)));
            } return rootFrame().Navigate(pageType, nullptr, EntranceNavigationTransitionInfo());
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
    winrt::slim_mutex RootContainer::s_mutex;
    RootContainerMap RootContainer::s_instances;

    local::RootContainer RootContainer::Current()
    {
        winrt::slim_lock_guard lock(s_mutex);
        auto view = ApplicationView::GetForCurrentView();
        int viewId = view.Id();
            
        auto it = s_instances.find(viewId);
        if (it != s_instances.end())
        {
            if (auto instance = it->second.get())
            { return instance; }
            s_instances.erase(it);
        }
            
        auto newInstance = winrt::make<implementation::RootContainer>(view);
        s_instances.emplace(viewId, newInstance);
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
