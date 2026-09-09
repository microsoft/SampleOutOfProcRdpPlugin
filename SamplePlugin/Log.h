#pragma once
#include "pch.h"
#include <format>
#include <string>
#include <fstream>
#include <mutex>
#include <shlobj.h>   // SHGetKnownFolderPath, FOLDERID_Downloads

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")

// ----------------------------------------------------------------------------
// File-based logging.
// The plugin runs as an out-of-proc COM server with no interactive console, so
// instead of allocating a cmd window we append timestamped lines to a log file
// under the user's Downloads folder: <Downloads>\SamplePlugin\SamplePlugin.log
// (falls back to %USERPROFILE%\Downloads, then %TEMP% if Downloads can't be
// resolved / written under app isolation).
// ----------------------------------------------------------------------------

inline std::string NarrowUtf8(const std::wstring& w)
{
    if (w.empty()) return {};
    int n = ::WideCharToMultiByte(CP_UTF8, 0, w.c_str(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<size_t>(n), '\0');
    ::WideCharToMultiByte(CP_UTF8, 0, w.c_str(), static_cast<int>(w.size()), s.data(), n, nullptr, nullptr);
    return s;
}

inline std::wstring GetLogFilePath()
{
    std::wstring dir;

    PWSTR downloads = nullptr;
    if (SUCCEEDED(::SHGetKnownFolderPath(FOLDERID_Downloads, 0, nullptr, &downloads)) && downloads)
    {
        dir.assign(downloads);
        ::CoTaskMemFree(downloads);
    }

    if (dir.empty())
    {
        wchar_t* userProfile = nullptr; size_t len = 0;
        if (_wdupenv_s(&userProfile, &len, L"USERPROFILE") == 0 && userProfile)
        {
            dir.assign(userProfile);
            dir += L"\\Downloads";
            free(userProfile);
        }
    }

    if (dir.empty())
    {
        wchar_t tmp[MAX_PATH]{};
        if (::GetTempPathW(MAX_PATH, tmp)) dir.assign(tmp);
    }

    dir += L"\\SamplePlugin";
    ::CreateDirectoryW(dir.c_str(), nullptr); // succeeds or already-exists; ignore result
    return dir + L"\\SamplePlugin.log";
}

inline std::string CurrentTimestamp()
{
    SYSTEMTIME st{}; ::GetLocalTime(&st);
    char buf[32]{};
    sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return buf;
}

inline void WriteLogLine(const std::string& text)
{
    static std::mutex logMutex;
    // Opened once (append). MSVC std::ofstream accepts a wide path, so Unicode
    // user names in the path are handled correctly.
    static std::ofstream logFile(GetLogFilePath().c_str(), std::ios::app);

    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open())
    {
        logFile << "[" << CurrentTimestamp() << "] " << text << "\n";
        logFile.flush();
    }
}

inline void Log(const std::string& message) { WriteLogLine(message); }
inline void Log(const char* message)         { WriteLogLine(message ? std::string(message) : std::string()); }
inline void Log(const std::wstring& message) { WriteLogLine(NarrowUtf8(message)); }
inline void Log(const wchar_t* message)      { WriteLogLine(NarrowUtf8(message ? std::wstring(message) : std::wstring())); }

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