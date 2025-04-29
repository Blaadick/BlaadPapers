#pragma once

#include <fstream>
#include <iostream>
#include <QScreen>
#include "ConfigManager.hpp"
#include "Wallpaper.hpp"

//TODO move away
inline void setWallpaper(const std::string& monitorName, const Wallpaper& wallpaper) {
    std::string wallpaperFilePath = wallpaper.getFilePath().string();

    system(("hyprctl -q hyprpaper preload " + wallpaperFilePath).c_str());
    system(("hyprctl -q hyprpaper wallpaper , " + wallpaperFilePath).c_str());

    std::ofstream hyprpaperConfig(((std::string(getenv("HOME")) + "/.config/hypr/hyprpaper.conf").c_str()));
    hyprpaperConfig << "preload = " << wallpaperFilePath << std::endl;
    hyprpaperConfig << "wallpaper = , " << wallpaperFilePath << std::endl;
    hyprpaperConfig.close();

    const std::filesystem::path postScriptPath = ConfigManager::getConfigDir() / "post.sh";
    std::cout << postScriptPath;
    if(exists(postScriptPath)) {
        system(("bash " + postScriptPath.string() + " \"" + wallpaperFilePath + "\"").c_str());
    }
}

inline QSize getAspectRatio(const QScreen* screen) {
    const int divisor = std::gcd(screen->geometry().width(), screen->geometry().height());
    return screen->geometry().size() / divisor;
}
