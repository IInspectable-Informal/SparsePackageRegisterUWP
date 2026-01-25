#pragma once
#include "Data.TaskInfo.g.h"

namespace winrt::SparsePackageManager::Data::implementation
{
    using DeploymentTask = Windows::Foundation::IAsyncOperationWithProgress<Windows::Management::Deployment::DeploymentResult, Windows::Management::Deployment::DeploymentProgress>;

    enum class TaskType
    {
        Add,
        Register,
        Remove
    };

    struct TaskInfo : TaskInfoT<TaskInfo>
    {
    public:
        TaskInfo(TaskType, DeploymentTask const&);

        //Methods
        Windows::Foundation::IAsyncOperation<bool> RunTaskAsync();

        //Properties
        hstring Summary();
        hstring Detail();
        bool IsWaiting();
        double Percentage();
        locald::TaskState State();

        localc::CloseRequestedCallback RemovalCallback();

        //INotifyPropertyChanged
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const&)noexcept;
        void PropertyChanged(event_token const&)noexcept;

        ~TaskInfo();

        //Static Methods
        static locald::TaskInfo CreateInstance(Windows::Foundation::Uri const&, Windows::Management::Deployment::AddPackageOptions const&);
        static locald::TaskInfo CreateInstance(Windows::Foundation::Uri const&, Windows::Management::Deployment::RegisterPackageOptions const&);
        static locald::TaskInfo CreateInstance(hstring const&);

    protected:
        hstring m_Source;
        hstring m_ExternalPath;

    private:
        const TaskType m_TaskType;
        const DeploymentTask m_DeploymentTask;
        const Windows::UI::Core::CoreDispatcher m_Dispatcher;

        hstring m_Summary;
        hstring m_Detail;
        bool m_IsWaiting = true;
        double m_Percentage = 0;
        locald::TaskState m_State = locald::TaskState::Working;
        localc::CloseRequestedCallback m_RemovalCallback = nullptr;

        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;

        void Summary(hstring const&);
        void Detail(hstring const&);
        void IsWaiting(bool);
        void Percentage(double);
        void State(locald::TaskState);
        void Notify(hstring const& = L"")noexcept;
    };
}

namespace winrt::SparsePackageManager::Data::factory_implementation
{
    struct TaskInfo : TaskInfoT<TaskInfo, implementation::TaskInfo>
    {
    };
}
