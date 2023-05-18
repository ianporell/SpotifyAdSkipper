#include <windows.h>
#include <thread>
#include <chrono>
#include <shlwapi.h>
#include <string>

#pragma comment(lib, "User32.lib")

bool ad_playing() {
    return FindWindow(NULL, L"Advertisement") != NULL;
}

void restart_spotify(std::wstring path) {
    //fix this later
    system("taskkill /IM spotify.exe -f");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    //this should start spotify without unfocusing the current window which is annoying
    ShellExecute(NULL, L"open", path, NULL, NULL, SW_NORMAL);
}

void unpause_song() {
    //If you are using this program, you should have the free version
    HWND spotifyHwnd = FindWindow(NULL, L"Spotify Free");

    //dispatch space (unpause)
    PostMessage(spotifyHwnd, WM_KEYDOWN, VK_SPACE, 0);
    PostMessage(spotifyHwnd, WM_KEYUP, VK_SPACE, 0);
}
