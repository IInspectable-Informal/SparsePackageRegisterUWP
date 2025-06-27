#include "pch.h"
#include "Dialog.h"
#include "Dialog.g.cpp"

namespace winrt::SparsePackageRegisterUWP::implementation
{
    BOOL Dialog::Show(hstring const& name)
    {
        ResetContent(); UIElement Control = nullptr;
        if (name == L"Status") { Control = StatusPanel(); }
        else if (name == L"Inf") { Control = InfPresenter(); }
        else if (name == L"ErrorInf")
        {
            Control = ErrorPresenter();
            ScrV().VerticalScrollMode(ScrollMode::Disabled);
        }
        else { return false; }
        Control.Visibility(Visibility::Visible);
        return true;
    }

    void Dialog::SetDefaultButton(ContentDialogButton const& Button)
    {
        DefaultButton(Button);
        switch (Button)
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

    IAsyncOperation<ContentDialogResult> Dialog::ShowInf(hstring const& v_Inf, hstring const& v_PrimaryButton, hstring const& v_CloseButton, ContentDialogButton const& DefaultButton)
    {
        Show(L"Inf"); PrimaryButtonText(v_PrimaryButton); CloseButtonText(v_CloseButton);
        SetDefaultButton(DefaultButton);
        Inf(v_Inf);
        return PShow(*this);
    }

    IAsyncOperation<ContentDialogResult> Dialog::ShowError(hstring const& v_ErrorInf, hstring const& v_ErrorHeader)
    {
        Glyph(L"\xE783"); Text(L"错误"); Show(L"ErrorInf"); CloseButtonText(L"关闭");
        SetDefaultButton(ContentDialogButton::Close);
        ErrorInf(v_ErrorInf); ErrorHeader(v_ErrorHeader);
        return PShow(*this);
    }

    IAsyncOperation<ContentDialogResult> Dialog::ShowStatus(ContentDialogButton const& DefaultButton)
    {
        Show(L"Status"); SetDefaultButton(DefaultButton);
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
        m_Status = m_Inf = m_ErrorInf = m_ErrorHeader = L"";
        Notify();
    }

    Dialog::Dialog()
    {
        auto Res = Application::Current().Resources();
        DefaultButtonStyle = Res.Lookup(box_value(L"DefaultButtonStyle")).as<::Style>();
        AccentButtonStyle = Res.Lookup(box_value(L"AccentButtonStyle")).as<::Style>();
        Style(Res.Lookup(box_value(L"DefaultContentDialogStyle")).as<::Style>());
    }
}
