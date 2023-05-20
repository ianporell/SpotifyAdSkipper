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
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (ad_playing()) {
            //wait so that next song is played
            std::this_thread::sleep_for(std::chrono::seconds(2));

            //convert path to a lpcwstr
            std::wstring wPath(path.begin(), path.end());
            LPCWSTR cwPath = wPath.c_str();

            restart_spotify(cwPath);

            std::this_thread::sleep_for(std::chrono::seconds(1));
            unpause_song();

            std::cout << "Advertisement skipped" << std::endl;
        }
    }
}


