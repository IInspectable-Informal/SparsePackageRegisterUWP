#include "pch.h"
#include "Data/TaskInfo.h"
#include "Data.TaskInfo.g.cpp"
#include "helpers.hpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Management::Deployment;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Data;

namespace winrt::SparsePackageManager::Data::implementation
{
    TaskInfo::TaskInfo(TaskType type, DeploymentTask const& deploymentTask) :
        m_TaskType(type),
        m_DeploymentTask(deploymentTask),
        m_Dispatcher(CoreWindow::GetForCurrentThread().Dispatcher())
    {
        m_RemovalCallback = 
        [this]()
        {
            auto list = GetTaskInfoListForCurrentThread();
            uint32_t i;
            if (list.IndexOf(*this, i))
            { list.RemoveAt(i); }
        };
        switch (type)
        {
            case TaskType::Add:
            {
                m_DeploymentTask.Progress(
                [this](DeploymentTask const&, DeploymentProgress const& e) -> fire_and_forget
                {
                    co_await resume_foreground(m_Dispatcher);
                    bool isProcessing = e.state == DeploymentProgressState::Processing;
                    if (isProcessing) { Percentage(e.percentage); }
                    IsWaiting(!isProcessing);
                    Detail(
                        runtime_format(
                            GetLocalizedString(isProcessing ? L"AddProgressMessage" : L"AddProgressWaitingMessage"),
                            m_ExternalPath, m_Source, Percentage()
                        )
                    );
                });
            } break;
            case TaskType::Register:
            {
                m_DeploymentTask.Progress(
                [this](DeploymentTask const&, DeploymentProgress const& e) -> fire_and_forget
                {
                    co_await resume_foreground(m_Dispatcher);
                    bool isProcessing = e.state == DeploymentProgressState::Processing;
                    if (isProcessing) { Percentage(e.percentage); }
                    IsWaiting(!isProcessing);
                    Detail(
                        runtime_format(
                            GetLocalizedString(isProcessing ? L"RegisterProgressMessage" : L"RegisterProgressWaitingMessage"),
                            m_ExternalPath, m_Source, Percentage()
                        )
                    );
                });
            } break;
            case TaskType::Remove:
            {
                m_DeploymentTask.Progress(
                [this](DeploymentTask const&, DeploymentProgress const& e) -> fire_and_forget
                {
                    co_await resume_foreground(m_Dispatcher);
                    bool isProcessing = e.state == DeploymentProgressState::Processing;
                    if (isProcessing) { Percentage(e.percentage); }
                    IsWaiting(!isProcessing);
                    Detail(
                        runtime_format(
                            GetLocalizedString(isProcessing ? L"RemoveProgressMessage" : L"RemoveProgressWaitingMessage"),
                            m_Source, Percentage()
                        )
                    );
                });
            } break;
        }
    }

    IAsyncOperation<bool> TaskInfo::RunTaskAsync()
    {
        try
        {
            Summary(GetLocalizedString(m_TaskType != TaskType::Remove ? L"RegisteringTitle" : L"RemovingTitle"));
            switch (m_TaskType)
            {
                case TaskType::Add:
                {
                    Detail(runtime_format(GetLocalizedString(L"AddStartingMessage"), m_ExternalPath, m_Source));
                } break;
                case TaskType::Register:
                {
                    Detail(runtime_format(GetLocalizedString(L"RegisterStartingMessage"), m_ExternalPath, m_Source));
                } break;
                case TaskType::Remove:
                {
                    Detail(runtime_format(GetLocalizedString(L"RemoveStartingMessage"), m_Source));
                } break;
            }
            auto result = co_await m_DeploymentTask;
            bool succeeded = result.IsRegistered();
            if (succeeded)
            {
                State(locald::TaskState::Succeeded);
                switch (m_TaskType)
                {
                    case TaskType::Add:
                    {
                        Summary(GetLocalizedString(L"RegisterSucceededTitle"));
                        Detail(runtime_format(GetLocalizedString(L"AddSucceededMessage"), m_ExternalPath, m_Source));
                    } break;
                    case TaskType::Register:
                    {
                        Summary(GetLocalizedString(L"RegisterSucceededTitle"));
                        Detail(runtime_format(GetLocalizedString(L"RegisterSucceededMessage"), m_ExternalPath, m_Source));
                    } break;
                    case TaskType::Remove:
                    {
                        Summary(GetLocalizedString(L"RemoveSucceededTitle"));
                        Detail(runtime_format(GetLocalizedString(L"RemoveSucceededMessage"), m_Source));
                    } break;
                }
            }
            else
            {
                State(locald::TaskState::Failed);
                Summary(GetLocalizedString(m_TaskType != TaskType::Remove ? L"RegisterFailedTitle" : L"RemoveFailedTitle"));
                switch (m_TaskType)
                {
                    case TaskType::Add:
                    {
                        Detail(
                            runtime_format(
                                GetLocalizedString(L"AddFailedMessage"),
                                m_ExternalPath, m_Source, result.ErrorText(), result.ExtendedErrorCode(), result.ActivityId()
                            )
                        );
                    } break;
                    case TaskType::Register:
                    {
                        Detail(
                            runtime_format(
                                GetLocalizedString(L"RegisterFailedMessage"),
                                m_ExternalPath, m_Source, result.ErrorText(), result.ExtendedErrorCode(), result.ActivityId()
                            )
                        );
                    } break;
                    case TaskType::Remove:
                    {
                        Detail(
                            runtime_format(
                                GetLocalizedString(L"RemoveFailedMessage"),
                                m_Source, result.ErrorText(), result.ExtendedErrorCode(), result.ActivityId()
                            )
                        );
                    } break;
                }
            } co_return succeeded;
        }
        catch (hresult_error const& ex)
        {
            State(locald::TaskState::Failed);
            Summary(GetLocalizedString(m_TaskType != TaskType::Remove ? L"RegisterErrorTitle" : L"RemoveErrorTitle"));
            switch (m_TaskType)
            {
                case TaskType::Add:
                {
                    Detail(
                        runtime_format(
                            GetLocalizedString(L"AddErrorMessage"),
                            m_ExternalPath, m_Source, ex.message(), ex.code()
                        )
                    );
                } break;
                case TaskType::Register:
                {
                    Detail(
                        runtime_format(
                            GetLocalizedString(L"RegisterErrorMessage"),
                            m_ExternalPath, m_Source, ex.message(), ex.code()
                        )
                    );
                } break;
                case TaskType::Remove:
                {
                    Detail(
                        runtime_format(
                            GetLocalizedString(L"RemoveErrorMessage"),
                            m_Source, ex.message(), ex.code()
                        )
                    );
                } break;
            }
        } co_return false;
    }

    //Static Methods
    locald::TaskInfo TaskInfo::CreateInstance(Uri const& packageUri, AddPackageOptions const& options)
    {
        DeploymentTask task = GetPackageManagerForCurrentThread()->AddPackageByUriAsync(packageUri, options);
        if (task)
        {
            auto self = make_self<implementation::TaskInfo>(TaskType::Add, task);
            self->m_Source = packageUri.RawUri();
            self->m_ExternalPath = options.ExternalLocationUri().RawUri();
            return self.as<locald::TaskInfo>();
        } else { return nullptr; }
    }

    locald::TaskInfo TaskInfo::CreateInstance(Uri const& manifestUri, RegisterPackageOptions const& options)
    {
        DeploymentTask task = GetPackageManagerForCurrentThread()->RegisterPackageByUriAsync(manifestUri, options);
        if (task)
        {
            auto self = make_self<implementation::TaskInfo>(TaskType::Register, task);
            self->m_Source = manifestUri.RawUri();
            self->m_ExternalPath = options.ExternalLocationUri().RawUri();
            return self.as<locald::TaskInfo>();
        } else { return nullptr; }
    }

    locald::TaskInfo TaskInfo::CreateInstance(hstring const& packageFullName)
    {
        DeploymentTask task = GetPackageManagerForCurrentThread()->RemovePackageAsync(packageFullName);
        if (task)
        {
            auto self = make_self<implementation::TaskInfo>(TaskType::Remove, task);
            self->m_Source = packageFullName;
            return self.as<locald::TaskInfo>();
        } else { return nullptr; }
    }

    //Properties
    hstring TaskInfo::Summary()
    { return m_Summary; }

    hstring TaskInfo::Detail()
    { return m_Detail; }

    bool TaskInfo::IsWaiting()
    { return m_IsWaiting; }

    double TaskInfo::Percentage()
    { return m_Percentage; }

    locald::TaskState TaskInfo::State()
    { return m_State; }

    localc::CloseRequestedCallback TaskInfo::RemovalCallback()
    { return m_RemovalCallback; }

    //INotifyPropertyChanged
    event_token TaskInfo::PropertyChanged(PropertyChangedEventHandler const& handler) noexcept
    { return m_PropertyChanged.add(handler); }
    void TaskInfo::PropertyChanged(event_token const& token) noexcept
    { m_PropertyChanged.remove(token); }

    //Private Methods
    void TaskInfo::Summary(hstring const& value)
    { m_Summary = value; Notify(L"Summary"); }

    void TaskInfo::Detail(hstring const& value)
    { m_Detail = value; Notify(L"Detail"); }

    void TaskInfo::IsWaiting(bool value)
    { m_IsWaiting = value; Notify(L"IsWaiting"); }

    void TaskInfo::Percentage(double value)
    { m_Percentage = value; Notify(L"Percentage"); }

    void TaskInfo::State(locald::TaskState value)
    { m_State = value; Notify(L"State"); }

    void TaskInfo::TaskInfo::Notify(hstring const& propertyName) noexcept
    { m_PropertyChanged(*this, PropertyChangedEventArgs(propertyName)); }

    //Destructor
    TaskInfo::~TaskInfo()
    {
        m_PropertyChanged.clear();
    }
}
