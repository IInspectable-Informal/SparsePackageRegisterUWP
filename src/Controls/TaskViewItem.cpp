#include "pch.h"
#include "Controls/TaskViewItem.h"
#include "Controls.TaskViewItem.g.cpp"
#include "helpers.hpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Management::Deployment;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace winrt::SparsePackageManager::Controls::implementation
{
    TaskViewItem::TaskViewItem() :
        m_CloseButtonClickHandler({ this, &TaskViewItem::CloseButtonClick })
    {
        DefaultStyleKey(box_value(TaskViewItem::s_Type));
    }

    //FrameworkElement
    void TaskViewItem::OnApplyTemplate()
    {
        TryRemoveBindings();
        m_RootGrid = GetTemplateChild(L"RootGrid").as<Grid>();
        m_CloseButton = GetTemplateChild(L"CloseButton").as<Button>();
        ToolTipService::SetToolTip(m_CloseButton, box_value(GetLocalizedString(L"CloseButtonText")));
        m_CloseButton.Click(m_CloseButtonClickHandler);
    }

    //Private Functions
    void TaskViewItem::CloseButtonClick(IInspectable const&, RoutedEventArgs const&)
    { m_CloseRequested(); }

    void TaskViewItem::UpdateVisualStateRequested(IInspectable const&, RoutedEventArgs const&)
    {
        UpdateState();
        Loaded(m_LoadedToken);
        m_LoadedToken.value = 0;
    }

    void TaskViewItem::TryRemoveBindings()
    {
        if (m_CloseButton)
        {
            m_CloseButton.Click(m_CloseButtonClickToken);
            ToolTipService::SetToolTip(m_CloseButton, nullptr);
            m_CloseButton = nullptr;
        }
        if (m_RootGrid)
        { m_RootGrid = nullptr; }
    }

    bool TaskViewItem::UpdateState()
    {
        switch (m_State)
        {
            case locald::TaskState::Working:
                return VisualStateManager::GoToState(*this, L"Infomational", false);
            case locald::TaskState::Succeeded:
                return VisualStateManager::GoToState(*this, L"Success", false);
            case locald::TaskState::Failed:
                return VisualStateManager::GoToState(*this, L"Error", false);
        }
    }

    //Properties
    hstring TaskViewItem::Title()
    { return GetValue(TaskViewItem::s_TitleProperty).as<hstring>(); }
    void TaskViewItem::Title(hstring const& value)
    { SetValue(TaskViewItem::s_TitleProperty, box_value(value)); }

    hstring TaskViewItem::Message()
    { return GetValue(TaskViewItem::s_MessageProperty).as<hstring>(); }
    void TaskViewItem::Message(hstring const& value)
    { SetValue(TaskViewItem::s_MessageProperty, box_value(value)); }

    double TaskViewItem::Percentage()
    { return GetValue(TaskViewItem::s_PercentageProperty).as<double>(); }
    void TaskViewItem::Percentage(double value)
    { SetValue(TaskViewItem::s_PercentageProperty, box_value(value)); }

    bool TaskViewItem::IsIndeterminate()
    { return GetValue(TaskViewItem::s_IsIndeterminateProperty).as<bool>(); }
    void TaskViewItem::IsIndeterminate(bool value)
    { SetValue(TaskViewItem::s_IsIndeterminateProperty, box_value(value)); }

    //Static Properties
    DependencyProperty TaskViewItem::TitleProperty()
    { return TaskViewItem::s_TitleProperty; }

    DependencyProperty TaskViewItem::MessageProperty()
    { return TaskViewItem::s_MessageProperty; }

    DependencyProperty TaskViewItem::PercentageProperty()
    { return TaskViewItem::s_PercentageProperty; }

    DependencyProperty TaskViewItem::IsIndeterminateProperty()
    { return TaskViewItem::s_IsIndeterminateProperty; }

    locald::TaskState TaskViewItem::State()
    { return m_State; }
    void TaskViewItem::State(locald::TaskState value)
    {
        m_State = value;
        if (IsLoaded())
        { UpdateState(); }
        else if (m_LoadedToken.value == 0)
        { m_LoadedToken = Loaded({ this, &TaskViewItem::UpdateVisualStateRequested }); }
    }

    localc::CloseRequestedCallback TaskViewItem::CloseRequested()
    { return m_CloseRequested; }
    void TaskViewItem::CloseRequested(localc::CloseRequestedCallback const& value)
    { m_CloseRequested = value; }

    //Destructor
    TaskViewItem::~TaskViewItem()
    {
        TryRemoveBindings();
        if (m_LoadedToken.value != 0)
        { Loaded(m_LoadedToken); }
    }

    //Static Variables
    const ::Interop::TypeName TaskViewItem::s_Type = xaml_typename<localc::TaskViewItem>();

    const DependencyProperty TaskViewItem::s_TitleProperty = DependencyProperty::Register(
       L"Title",
       xaml_typename<hstring>(),
       TaskViewItem::s_Type,
       PropertyMetadata(box_value(L"")));

    const DependencyProperty TaskViewItem::s_MessageProperty = DependencyProperty::Register(
       L"Message",
       xaml_typename<hstring>(),
       TaskViewItem::s_Type,
       PropertyMetadata(box_value(L"")));

    const DependencyProperty TaskViewItem::s_PercentageProperty = DependencyProperty::Register(
       L"Percentage",
       xaml_typename<double>(),
       TaskViewItem::s_Type,
       PropertyMetadata(box_value(0.0)));

    const DependencyProperty TaskViewItem::s_IsIndeterminateProperty = DependencyProperty::Register(
       L"IsIndeterminate",
       xaml_typename<bool>(),
       TaskViewItem::s_Type,
       PropertyMetadata(box_value(true)));
}
