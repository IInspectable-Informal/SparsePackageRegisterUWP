#include "pch.h"
#include "Dialog.h"
#include "Dialog.g.cpp"

using namespace winrt;
using namespace Windows::UI::ViewManagement;

namespace winrt::SparsePackageManager::implementation
{
    bool Dialog::Show(hstring const& name)
    {
        ResetContent(); UIElement Control = nullptr;
        if (name == L"Status") { Control = StatusPanel(); }
        else if (name == L"Info") { Control = InfoPresenter(); }
        else if (name == L"ErrorInfo")
        {
            Control = ErrorPresenter();
            ScrV().VerticalScrollMode(ScrollMode::Disabled);
        }
        else { return false; }
        Control.Visibility(Visibility::Visible);
        return true;
    }

    void Dialog::SetDefaultButton(ContentDialogButton const& button)
    {
        DefaultButton(button);
        switch (button)
        {
            case ContentDialogButton::Primary: PrimaryButtonStyle(AccentButtonStyle); break;
            case ContentDialogButton::Secondary: SecondaryButtonStyle(AccentButtonStyle); break;
            case ContentDialogButton::Close: CloseButtonStyle(AccentButtonStyle); break;
            default: 
                PrimaryButtonStyle(DefaultButtonStyle);
                SecondaryButtonStyle(DefaultButtonStyle);
                CloseButtonStyle(DefaultButtonStyle);
                break;
        }
    }

    IAsyncOperation<ContentDialogResult> Dialog::ShowInfoAsync(hstring const& info, hstring const& primaryButton, hstring const& closeButton, ContentDialogButton const& defaultButton)
    {
        Show(L"Info"); PrimaryButtonText(primaryButton); CloseButtonText(closeButton);
        SetDefaultButton(defaultButton);
        Info(info);
        return PShow(*this);
    }

    IAsyncOperation<ContentDialogResult> Dialog::ShowErrorAsync(hstring const& errorInfo, hstring const& errorHeader)
    {
        Glyph(L"\xE783"); Text(L"错误"); Show(L"ErrorInfo"); CloseButtonText(L"关闭");
        SetDefaultButton(ContentDialogButton::Close);
        ErrorInfo(errorInfo); ErrorHeader(errorHeader);
        return PShow(*this);
    }

    IAsyncOperation<ContentDialogResult> Dialog::ShowStatusAsync(ContentDialogButton const& defaultButton)
    {
        Show(L"Status"); SetDefaultButton(defaultButton);
        return PShow(*this);
    }

    void Dialog::Reset(ContentDialog const&, ContentDialogClosedEventArgs const&)
    {
        Glyph(L""); Text(L"");
        PrimaryButtonText(L"");
        SecondaryButtonText(L"");
        CloseButtonText(L"");
        SetDefaultButton();
        ResetContent();
    }

    void Dialog::ResetContent()
    {
        for (UIElement const& item : Root().Children())
        { item.Visibility(Visibility::Collapsed); }
        ScrV().VerticalScrollMode(ScrollMode::Auto);
        m_Progress = 0; m_IsIndeterminate = false;
        m_Status = m_Info = m_ErrorInfo = m_ErrorHeader = L"";
        Notify();
    }

    Dialog::Dialog()
    {
        auto Res = Application::Current().Resources();
        DefaultButtonStyle = Res.Lookup(box_value(L"DefaultButtonStyle")).as<::Style>();
        AccentButtonStyle = Res.Lookup(box_value(L"AccentButtonStyle")).as<::Style>();
        Style(Res.Lookup(box_value(L"DefaultContentDialogStyle")).as<::Style>());
    }

    //Static Properties
    winrt::slim_mutex Dialog::sm_mutex;
    DialogMap Dialog::sm_instances;

    local::Dialog Dialog::Current()
    {
        winrt::slim_lock_guard lock(sm_mutex);
        int viewId = ApplicationView::GetForCurrentView().Id();
            
        auto it = sm_instances.find(viewId);
        if (it != sm_instances.end())
        {
            if (auto instance = it->second.get())
            { return instance; }
            sm_instances.erase(it);
        }
            
        auto newInstance = winrt::make<implementation::Dialog>();
        sm_instances.emplace(viewId, newInstance);
        return newInstance;
    }
}
