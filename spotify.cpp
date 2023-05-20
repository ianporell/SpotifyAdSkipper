#include <windows.h>
#include <thread>
#include <chrono>
#include <shlwapi.h>
#include <string>
#include <cstring>
#include <psapi.h>
#include <vector>
#include <algorithm>
#include <iostream>

#pragma comment(lib, "User32.lib")
bool is_spotify_process(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess != NULL) {
        constexpr int bufferSize = 256;
        wchar_t processPath[bufferSize];
        if (GetModuleFileNameExW(hProcess, nullptr, processPath, bufferSize) > 0) {
            std::wstring path(processPath);
            std::transform(path.begin(), path.end(), path.begin(), ::tolower);
            if (path.find(L"spotify.exe") != std::wstring::npos) {
                return true;
            }
        }
        CloseHandle(hProcess);
    }

    return false;
}

HWND find_spotify_window() {
    std::vector<DWORD> processIds;
    constexpr DWORD bufferSize = 1024;
    DWORD processes[bufferSize];
    DWORD bytesReturned;

    if (EnumProcesses(processes, sizeof(processes), &bytesReturned)) {
        const DWORD count = bytesReturned / sizeof(DWORD);
        for (DWORD i = 0; i < count; ++i) {
            processIds.push_back(processes[i]);
        }
    }

    HWND spotifyHwnd = nullptr;
    HWND currentHwnd = nullptr;

    while ((currentHwnd = FindWindowEx(nullptr, currentHwnd, L"Chrome_WidgetWin_0", nullptr)) != nullptr) {
        DWORD processId;
        GetWindowThreadProcessId(currentHwnd, &processId);

        if (is_spotify_process(processId)) {
            spotifyHwnd = currentHwnd;
            break;
        }
    }

    return spotifyHwnd;
}


bool ad_playing() {
    HWND spotifyHwnd = find_spotify_window();

    const int bufferSize = 256;
    char windowTitle[bufferSize];
    GetWindowTextA(spotifyHwnd, windowTitle, bufferSize);

    std::cout << windowTitle << std::endl;

    if (strcmp(windowTitle, "Spotify Free") == NULL || windowTitle == "\0") {
        return false;
    }

    std::cout << (std::strchr(windowTitle, '-') == nullptr) << std::endl;
    return std::strchr(windowTitle, '-') == nullptr;
}

void restart_spotify(LPCWSTR path) {
    //fix this later
    system("taskkill /IM spotify.exe -f");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    //this should start spotify without unfocusing the current window which is annoying
    ShellExecute(NULL, L"open", path, NULL, NULL, SW_MINIMIZE);
}

void unpause_song() {
    //if you are using this program, you should have the free version
    HWND spotifyHwnd = FindWindow(NULL, L"Spotify Free");

    //dispatch space (unpause)
    PostMessage(spotifyHwnd, WM_KEYDOWN, VK_SPACE, 0);
    PostMessage(spotifyHwnd, WM_KEYUP, VK_SPACE, 0);
}
