#pragma once

#include <fstream>
#include <QRect>
#include "Global.hpp"
#include "Wallpaper.hpp"

inline void setWallpaper(const std::string &monitorName, const Wallpaper &wallpaper) {
    //TODO move away
    system(("hyprctl -q hyprpaper preload \"" + wallpaper.getFilePath() + "\"").c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper.getFilePath() + "\"").c_str());

    std::ofstream hyprpaperConfig(((std::string(getenv("HOME")) + "/.config/hypr/hyprpaper.conf").c_str()));
    hyprpaperConfig << "preload = " << wallpaper.getFilePath() << std::endl;
    hyprpaperConfig << "wallpaper = , " << wallpaper.getFilePath() << std::endl;
    hyprpaperConfig.close();

    const std::filesystem::path postScriptPath = configDir / "post.sh";
    if(exists(postScriptPath)) {
        system(("bash " + postScriptPath.string() + " \"" + wallpaper.getFilePath() + "\"").c_str());
    }
}

inline QSize getAspectRatio(const QRect &geometry) {
    const int divisor = std::gcd(geometry.width(), geometry.height());
    return {geometry.width() / divisor, geometry.height() / divisor};
}
