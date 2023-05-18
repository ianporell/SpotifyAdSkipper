#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <filesystem>
#include "spotify.h"

int main() {
    std::string path = "";
   
    if (!std::filesystem::exists("config.txt")) {
        std::ofstream config("config.txt");
        std::cout << "Enter spotify exe path" << std::endl;
        std::cin >> path;
        config << path;
        config.close();
    }
    else {
        std::ifstream s("config.txt");
        while (std::getline(s, path));
    }
    std::cout << "Path: " << path << std::endl;

    std::cout << "Listening for advertisements..." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (ad_playing()) {
            //wait so that next song is played
            std::this_thread::sleep_for(std::chrono::seconds(4));

            //convert path to a wstring
            std::wstring wPath(path.length(), L'\0');
            std::copy(path.begin(), path.end(), &wPath);

            restart_spotify(wPath);

            std::this_thread::sleep_for(std::chrono::seconds(1));
            unpause_song();

            std::cout << "Advertisement skipped" << std::endl;
        }
    }
}


