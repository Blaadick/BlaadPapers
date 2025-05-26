#pragma once

#include <fstream>
#include <QScreen>
#include "Config.hpp"
#include "Wallpaper.hpp"

//TODO move away
inline void setWallpaper(const std::string& monitorName, const Wallpaper& wallpaper) {
    const std::string wallpaperFilePath = wallpaper.getFilePath().string();

    system(("hyprctl -q hyprpaper preload " + wallpaperFilePath).c_str());
    system(("hyprctl -q hyprpaper wallpaper , " + wallpaperFilePath).c_str());

    std::ofstream hyprpaperConfig(((std::string(getenv("HOME")) + "/.config/hypr/hyprpaper.conf").c_str()));
    hyprpaperConfig << "preload = " << wallpaperFilePath << std::endl;
    hyprpaperConfig << "wallpaper = , " << wallpaperFilePath << std::endl;
    hyprpaperConfig.close();

    if(exists(Config::getPostSetScriptPath())) {
        system(("bash " + Config::getPostSetScriptPath().string() + " \"" + wallpaperFilePath + "\"").c_str());
    }
}

inline QSize getAspectRatio(const QScreen* screen) {
    const int divisor = std::gcd(screen->geometry().width(), screen->geometry().height());
    return screen->geometry().size() / divisor;
}
