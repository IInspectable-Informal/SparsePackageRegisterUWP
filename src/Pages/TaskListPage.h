#pragma once
#include "Pages.TaskListPage.g.h"

namespace winrt::SparsePackageManager::Pages::implementation
{
    using PageMap = std::map<winrt::weak_ref<Windows::UI::Xaml::Controls::Frame>, localp::TaskListPage>;

    struct TaskListPage : TaskListPageT<TaskListPage>
    {
    public:
        TaskListPage();

        //Handlers
        void NavigateToPageRequested(IInspectable const&, Windows::UI::Xaml::RoutedEventArgs const&);

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
