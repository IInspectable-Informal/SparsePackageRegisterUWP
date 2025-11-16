#pragma once
#include "pch.h"
#include "Dialog.g.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
typedef TypedEventHandler<local::Dialog, IInspectable> DialogOpenedEventHandler;

namespace winrt::SparsePackageManager::implementation
{
    using DialogMap = std::map<int, winrt::weak_ref<local::Dialog>>;

    struct Dialog : DialogT<Dialog>
    {
        public:
            Dialog();

            bool Show(hstring const&);
            void SetDefaultButton(ContentDialogButton const& = ContentDialogButton::None);

            IAsyncOperation<ContentDialogResult> ShowInfoAsync(hstring const&, hstring const&, hstring const& = L"", ContentDialogButton const& = ContentDialogButton::None);
            IAsyncOperation<ContentDialogResult> ShowErrorAsync(hstring const&, hstring const& = L"");
            IAsyncOperation<ContentDialogResult> ShowStatusAsync(ContentDialogButton const&);

            event_token DialogOpened(DialogOpenedEventHandler const& handler)noexcept { return m_DialogOpened.add(handler); }
            void DialogOpened(event_token const& token)noexcept { m_DialogOpened.remove(token); }

            void Reset(ContentDialog const& = nullptr, ContentDialogClosedEventArgs const& = nullptr);
            void ResetContent();

            //XAML binding properties and events
            //Properties
            hstring Glyph() { return m_Glyph; }
            void Glyph(hstring const& value) { m_Glyph = value; Notify(); }
            hstring Text() { return m_Text; }
            void Text(hstring const& value) { m_Text = value; Notify(); }

            double Progress() { return m_Progress; }
            void Progress(double const& value) { m_Progress = value; Notify(); }
            bool IsIndeterminate() { return m_IsIndeterminate; }
            void IsIndeterminate(bool const& value) { m_IsIndeterminate = value; Notify(); }
            hstring Status() { return m_Status; }
            void Status(hstring const& value) { m_Status = value; Notify(); }

            hstring Info() { return m_Info; }
            void Info(hstring const& value) { m_Info = value; Notify(); }

            hstring ErrorInfo() { return m_ErrorInfo; }
            void ErrorInfo(hstring const& value) { m_ErrorInfo = value; Notify(); }
            hstring ErrorHeader() { return m_ErrorHeader; }
            void ErrorHeader(hstring const& value) { m_ErrorHeader = value; Notify(); }

            //Events
            void InvokeEvent(ContentDialog const&, ContentDialogOpenedEventArgs const&)
            { if (StatusPanel().Visibility() == Visibility::Visible) { m_DialogOpened(*this, nullptr); } }
            event_token PropertyChanged(PropertyChangedEventHandler const& eh)noexcept { return m_PropertyChanged.add(eh); }
            void PropertyChanged(event_token const& et)noexcept { m_PropertyChanged.remove(et); }

            //Static Properties
            static local::Dialog Current();

        private:
            ::Style DefaultButtonStyle{ nullptr };
            ::Style AccentButtonStyle{ nullptr };
            event<DialogOpenedEventHandler> m_DialogOpened;
            hstring m_Glyph; hstring m_Text;
            double m_Progress = 0; bool m_IsIndeterminate = false; hstring m_Status = L"";
            hstring m_Info = L"";
            hstring m_ErrorInfo = L""; hstring m_ErrorHeader = L"";
            event<PropertyChangedEventHandler> m_PropertyChanged;
            void Notify(hstring const& PropertyName = L"")noexcept
            { m_PropertyChanged(*this, PropertyChangedEventArgs(PropertyName)); }
            static IAsyncOperation<ContentDialogResult> PShow(ContentDialog const& v_Dialog) { return v_Dialog.ShowAsync(); }

            static winrt::slim_mutex sm_mutex;
            static DialogMap sm_instances;
    };
}

namespace winrt::SparsePackageManager::factory_implementation
{
    struct Dialog : DialogT<Dialog, implementation::Dialog>
    {
    };
}
