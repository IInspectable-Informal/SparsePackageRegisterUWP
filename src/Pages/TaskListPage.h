#pragma once
#include "Pages.TaskListPage.g.h"

namespace winrt::SparsePackageManager::Pages::implementation
{
    struct TaskListPage : TaskListPageT<TaskListPage>
    {
    public:
        TaskListPage();

        //Handlers
        void NavigateToPageRequested(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);
        void RemoveItemRequested(Microsoft::UI::Xaml::Controls::SwipeItem const&, Microsoft::UI::Xaml::Controls::SwipeItemInvokedEventArgs const&);

        //ITypeProvider
        Windows::UI::Xaml::Interop::TypeName Type();
    };
}

namespace winrt::SparsePackageManager::Pages::factory_implementation
{
    struct TaskListPage : TaskListPageT<TaskListPage, implementation::TaskListPage>
    {
    };
}
