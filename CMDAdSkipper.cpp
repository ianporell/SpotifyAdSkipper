#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#pragma comment(lib, "User32.lib")
#define PLAY_MEDIA 0xB3

INPUT play_media_input{
    INPUT_KEYBOARD,
    {
        PLAY_MEDIA,
        0,
        0,
        0,
        0
    }
};

bool ad_playing();
void restart_spotify();

int main()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (ad_playing()) restart_spotify();
    }
}

bool ad_playing() {
    return FindWindowA(NULL, "Advertisement") != NULL;
}

void restart_spotify() {
    system("taskkill /IM spotify.exe -f");
    system("C:\\Users\\chris\\AppData\\Roaming\\Spotify");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    SendInput(1, &play_media_input, sizeof(INPUT));

}
