#pragma once
#include "pch.h"
#include "App.xaml.g.h"

//WinRT
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.System.Profile.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::System::Profile;
using namespace Windows::Storage;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace winrt::SparsePackageRegisterUWP::implementation
{
    struct App : AppT<App>
    {
        public:
            App();
            void OnLaunched(LaunchActivatedEventArgs const&);
            void OnSuspending(IInspectable const&, SuspendingEventArgs const&);
        private:
            BOOL Inited = false;
            fire_and_forget CreateView();
            int InitView();
    };
}
