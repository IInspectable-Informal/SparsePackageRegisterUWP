#pragma once
#include "helpers.h"

template<>
struct std::formatter<winrt::guid, wchar_t>
{
public:
    constexpr auto parse(std::wformat_parse_context& ctx)
    {
        auto it =  ctx.begin();
        if (it != ctx.end())
        {
            wchar_t fmtchar = *it;
            if (fmtchar == L'U' || fmtchar == L'u')
            {
                m_IsUpper = fmtchar == L'U';
                ++it;
            }
        }
        if (it != ctx.end() && *it != L'}')
        { throw std::format_error("invalid format args for winrt::guid"); }
        return it;
    }

    auto format(const winrt::guid& guid, std::wformat_context& ctx) const
    {
        if (m_IsUpper)
        {
            return std::format_to(ctx.out(), L"{:08X}-{:04X}-{:04X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
                guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1],
                guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        }
        else
        {
            return std::format_to(ctx.out(), L"{:08x}-{:04x}-{:04x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
                guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1],
                guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        }
    }

private:
    bool m_IsUpper = true;
};

template<>
struct std::formatter<winrt::hresult, wchar_t> : std::formatter<uint32_t, wchar_t> {};

namespace winrt
{
    template<typename... Args>
    hstring runtime_format(const hstring& format_str, const Args&... args)
    { return std::vformat(format_str.c_str(), std::make_wformat_args(args...)).c_str(); }
}
