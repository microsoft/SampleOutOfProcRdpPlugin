#pragma once
#include "pch.h"
#include <format>


inline void EnsureConsoleAllocated()
{
    static bool consoleAllocated = false;
    if (!consoleAllocated && !::GetConsoleWindow())
    {
        ::AllocConsole();
        FILE* fpOut = nullptr; freopen_s(&fpOut, "CONOUT$", "w", stdout);
        FILE* fpErr = nullptr; freopen_s(&fpErr, "CONOUT$", "w", stderr);
        FILE* fpIn = nullptr; freopen_s(&fpIn, "CONIN$", "r", stdin);

        consoleAllocated = true;
    }
}

inline void Log(const wchar_t* message)
{
    EnsureConsoleAllocated();
    wprintf(L"%s\n", message);
}

inline std::wstring GetErrorDescription(HRESULT hr)
{
    wchar_t* errorMsg = nullptr;
    std::wstring errorStr;

    if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&errorMsg), 0, nullptr) != 0)
    {
        errorStr = errorMsg;
        LocalFree(errorMsg);

        while (!errorStr.empty() && (errorStr.back() == L'\n' || errorStr.back() == L'\r'))
        {
            errorStr.pop_back();
        }
    }
    return errorStr;
}

inline std::wstring Log(const wchar_t* functionName, HRESULT hr)
{
    const wchar_t* statusText = SUCCEEDED(hr) ? L"succeeded" : L"failed";

    std::wstring errorDescription;
    if (!SUCCEEDED(hr))
    {
        errorDescription = GetErrorDescription(hr);
    }

    std::wstring message = std::format(L"{} {} with HRESULT 0x{:08X}{}",
        functionName,
        statusText,
        static_cast<unsigned int>(hr),
        (!errorDescription.empty() ? std::format(L": {}", errorDescription) : L""));

    Log(message.c_str());

    return message;
}