#pragma once
#include "RootContainer.g.h"

namespace winrt::SparsePackageManager::implementation
{
    using RootContainerMap = std::map<int, winrt::weak_ref<local::RootContainer>>;

    struct RootContainer : RootContainerT<RootContainer>
    {
    public:
        RootContainer(Windows::UI::ViewManagement::ApplicationView const&);

        //Handlers
        void ThemeChanged(FrameworkElement const&, IInspectable const&);

        void OnTitleBarLoaded(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        void NavViewDisplayModeChanged(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewDisplayModeChangedEventArgs const&);

        void NavigationRequested(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const&);
        void SelectedViewItemChanged(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const&);
        void GoBackRequested(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const&);
        void FrameNavigated(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationEventArgs const&);

        //Properties
        bool IsPaneOnTop();
        void IsPaneOnTop(bool const&);

        //Static Properties
        static local::RootContainer Current();

        static hstring AppName();
        static Windows::Foundation::Uri AppIcon();
        static hstring Developer();
        static hstring Version();

    private:
        const Windows::UI::ViewManagement::ApplicationViewTitleBar m_ViewTitleBar;

        const local::Dialog m_Dialog;

        void NavigateTo(hstring const&);

        static winrt::slim_mutex sm_mutex;
        static RootContainerMap sm_instances;
    };
}

namespace winrt::SparsePackageManager::factory_implementation
{
    struct RootContainer : RootContainerT<RootContainer, implementation::RootContainer>
    {
    };
}
