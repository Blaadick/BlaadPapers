#pragma once

#include <iostream>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include "Config.hpp"

inline void applyWallpaper(const QString& wallpaperName) {
    //TODO Remove when implement own renderer

    const Wallpaper* wallpaper = Wallpapers::getWallpaper(wallpaperName);

    if(!wallpaper) {
        std::cerr << "Wallpaper not found";
        return;
    }

    const QString& picturePath = wallpaper->getPicturePath();

    QFile hyprpaperConfig(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/hypr/hyprpaper.conf");

    system(("bash " + Config::getPostSetScriptPath() + " \"" + picturePath + '\"').toStdString().c_str());
    system("hyprctl -q hyprpaper unload all");
    system(("hyprctl -q hyprpaper preload \"" + picturePath + '\"').toStdString().c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + picturePath + '\"').toStdString().c_str());

    hyprpaperConfig.open(QIODevice::WriteOnly);
    hyprpaperConfig.write(("preload = " + picturePath + '\n').toStdString().c_str());
    hyprpaperConfig.write(("wallpaper = , " + picturePath).toStdString().c_str());
    hyprpaperConfig.close();
}
