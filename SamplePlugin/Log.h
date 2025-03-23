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

inline std::wstring Log(const wchar_t* functionName, HRESULT hr)
{
    auto message = std::format(L"{} failed with HRESULT 0x{:08X}", functionName, hr);
    Log(message.c_str());
    return message;
}