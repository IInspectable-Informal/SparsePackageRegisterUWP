#include "pch.h"
#include "Pages/TaskListPage.h"
#include "Pages.TaskListPage.g.cpp"
#include "constants.h"
#include "helpers.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
namespace muxc = Microsoft::UI::Xaml::Controls;

namespace winrt::SparsePackageManager::Pages::implementation
{
    TaskListPage::TaskListPage()
    {
        InitializeComponent();
        TaskViewList().ItemsSource(GetTaskInfoListForCurrentThread());
    }

    //Handlers
    void TaskListPage::NavigateToPageRequested(IInspectable const& sender, RoutedEventArgs const&)
    {
        switch (sender.as<FrameworkElement>().Tag().as<hstring>()[0])
        {
            case L'0':
            {
                local::RootContainer::Current().Navigate(g_RegisterPageType);
            } break;
            case L'1':
            {
                local::RootContainer::Current().Navigate(g_PackageListPageType);
            } break;
        }
    }

    void TaskListPage::RemoveItemRequested(muxc::SwipeItem const& sender, muxc::SwipeItemInvokedEventArgs const&)
    { sender.CommandParameter().as<locald::TaskInfo>().RemovalCallback()(); }

    //ITypeProvider
    TypeName TaskListPage::Type()
    { return g_TaskListPageType; }
}
