#pragma once
//WinRT
#include <winrt\Windows.Storage.h>

using namespace winrt::Windows::Storage;

namespace winrt
{
    const ApplicationDataContainer AppDC = ApplicationData::Current().RoamingSettings();
}