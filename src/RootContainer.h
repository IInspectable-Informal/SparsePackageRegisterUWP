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

        //INavigate
        bool Navigate(Windows::UI::Xaml::Interop::TypeName const&);

        //Static Properties
        static local::RootContainer Current();

        static hstring AppName();
        static Windows::Foundation::Uri AppIcon();
        static hstring Developer();
        static hstring Version();

    private:
        std::vector<Windows::UI::Xaml::Interop::TypeName> m_TypeList;
        const Windows::UI::ViewManagement::ApplicationViewTitleBar m_ViewTitleBar;
        const local::Dialog m_Dialog;

        static winrt::slim_mutex s_mutex;
        static RootContainerMap s_instances;
    };
}

namespace winrt::SparsePackageManager::factory_implementation
{
    struct RootContainer : RootContainerT<RootContainer, implementation::RootContainer>
    {
    };
}
