#pragma once
#include "pch.h"
#include "Dialog.g.h"

//WinRT
#include <winrt/Windows.UI.Xaml.Data.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
typedef TypedEventHandler<local::Dialog, IInspectable> ChangeStatusEventHandler;

namespace winrt::SprasePackageRegisterUWP::implementation
{
    struct Dialog : DialogT<Dialog>
    {
        public:
            Dialog();

            BOOL Show(hstring const&);
            void SetDefaultButton(ContentDialogButton const& = ContentDialogButton::None);

            IAsyncOperation<ContentDialogResult> ShowInf(hstring const&, hstring const&, hstring const& = L"", ContentDialogButton const& = ContentDialogButton::None);
            IAsyncOperation<ContentDialogResult> ShowError(hstring const&, hstring const& = L"");
            IAsyncOperation<ContentDialogResult> ShowStatus(ContentDialogButton const&);

            event_token ChangeStatus(ChangeStatusEventHandler const& eh)noexcept { return m_ChangeStatus.add(eh); }
            void ChangeStatus(event_token const& et)noexcept { m_ChangeStatus.remove(et); }

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
            BOOL IsIndeterminate() { return m_IsIndeterminate; }
            void IsIndeterminate(BOOL const& value) { m_IsIndeterminate = value; Notify(); }
            hstring Status() { return m_Status; }
            void Status(hstring const& value) { m_Status = value; Notify(); }

            hstring Inf() { return m_Inf; }
            void Inf(hstring const& value) { m_Inf = value; Notify(); }

            hstring ErrorInf() { return m_ErrorInf; }
            void ErrorInf(hstring const& value) { m_ErrorInf = value; Notify(); }
            hstring ErrorHeader() { return m_ErrorHeader; }
            void ErrorHeader(hstring const& value) { m_ErrorHeader = value; Notify(); }

            //Events
            void InvokeEvent(ContentDialog const&, ContentDialogOpenedEventArgs const&)
            { if (StatusPanel().Visibility() == Visibility::Visible) { m_ChangeStatus(*this, nullptr); } }
            event_token PropertyChanged(PropertyChangedEventHandler const& eh)noexcept { return m_PropertyChanged.add(eh); }
            void PropertyChanged(event_token const& et)noexcept { m_PropertyChanged.remove(et); }
        private:
            ::Style DefaultButtonStyle{ nullptr };
            ::Style AccentButtonStyle{ nullptr };
            event<ChangeStatusEventHandler> m_ChangeStatus;
            hstring m_Glyph; hstring m_Text;
            double m_Progress = 0; BOOL m_IsIndeterminate = false; hstring m_Status = L"";
            hstring m_Inf = L"";
            hstring m_ErrorInf = L""; hstring m_ErrorHeader = L"";
            event<PropertyChangedEventHandler> m_PropertyChanged;
            void Notify(hstring const& PropertyName = L"")noexcept
            { m_PropertyChanged(*this, PropertyChangedEventArgs(PropertyName)); }
            static IAsyncOperation<ContentDialogResult> PShow(ContentDialog const& v_Dialog) { return v_Dialog.ShowAsync(); }
    };
}

namespace winrt::SprasePackageRegisterUWP::factory_implementation
{
    struct Dialog : DialogT<Dialog, implementation::Dialog>
    {
    };
}
