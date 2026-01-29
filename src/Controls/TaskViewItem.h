#pragma once
#include "Controls.TaskViewItem.g.h"

namespace winrt::SparsePackageManager::Controls::implementation
{
    struct TaskViewItem : TaskViewItemT<TaskViewItem>
    {
    public:
        TaskViewItem();

        //Properties
        hstring Title();
        void Title(hstring const&);

        hstring Message();
        void Message(hstring const&);

        double Percentage();
        void Percentage(double);

        bool IsIndeterminate();
        void IsIndeterminate(bool);

        locald::TaskState State();
        void State(locald::TaskState);

        localc::CloseRequestedCallback CloseRequested();
        void CloseRequested(localc::CloseRequestedCallback const&);

        //FrameworkElement
        void OnApplyTemplate();

        ~TaskViewItem();

        //Static Properties
        static Windows::UI::Xaml::DependencyProperty TitleProperty();
        static Windows::UI::Xaml::DependencyProperty MessageProperty();
        static Windows::UI::Xaml::DependencyProperty PercentageProperty();
        static Windows::UI::Xaml::DependencyProperty IsIndeterminateProperty();

    private:
        locald::TaskState m_State;
        localc::CloseRequestedCallback m_CloseRequested{ nullptr };

        Windows::UI::Xaml::Controls::Grid m_RootGrid{ nullptr };
        Windows::UI::Xaml::Controls::Button m_CloseButton{ nullptr };
        const Windows::UI::Xaml::RoutedEventHandler m_CloseButtonClickHandler;
        event_token m_CloseButtonClickToken{0};
        event_token m_LoadedToken{0};

        void CloseButtonClick(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        void UpdateVisualStateRequested(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

        void TryRemoveBindings();
        bool UpdateState();

        static const Windows::UI::Xaml::Interop::TypeName s_Type;

        static const Windows::UI::Xaml::DependencyProperty s_TitleProperty;
        static const Windows::UI::Xaml::DependencyProperty s_MessageProperty;
        static const Windows::UI::Xaml::DependencyProperty s_PercentageProperty;
        static const Windows::UI::Xaml::DependencyProperty s_IsIndeterminateProperty;
    };
}

namespace winrt::SparsePackageManager::Controls::factory_implementation
{
    struct TaskViewItem : TaskViewItemT<TaskViewItem, implementation::TaskViewItem>
    {
    };
}
