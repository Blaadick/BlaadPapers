#pragma once

#include <fstream>
#include <QRect>
#include <QSizeF>

#include "Wallpaper.hpp"

inline void setWallpaper(const Wallpaper &wallpaper) {
    //TODO move away
    system(("hyprctl -q hyprpaper preload \"" + wallpaper.getFilePath() + "\"").c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper.getFilePath() + "\"").c_str());

    std::ofstream hyprpaperConfig(((std::pmr::string(getenv("HOME")) + "/.config/hypr/hyprpaper.conf").c_str()));
    hyprpaperConfig << "preload = " << wallpaper.getFilePath() << std::endl;
    hyprpaperConfig << "wallpaper = , " << wallpaper.getFilePath() << std::endl;
    hyprpaperConfig.close();
}

inline QSize getAspectRatio(const QRect &geometry) {
    const int divisor = std::gcd(geometry.width(), geometry.height());
    return {geometry.width() / divisor, geometry.height() / divisor};
}
