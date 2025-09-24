#pragma once

#include <QFile>
#include <QStandardPaths>
#include <QString>
#include "Config.hpp"
#include "util/Loggers.hpp"

namespace util {
    inline void applyWallpaper(const QString& wallpaperName) {
        //TODO Remove when implement own renderer

        const Wallpaper* wallpaper = Wallpapers::getWallpaper(wallpaperName);

        if(!wallpaper) {
            logError("Wallpaper not found");
            return;
        }

        QFile hyprpaperConfig(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/hypr/hyprpaper.conf");

        system(("bash " + Config::getPostSetScriptPath() + " \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
        system("hyprctl -q hyprpaper unload all");
        system(("hyprctl -q hyprpaper preload \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
        system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper->getFilePath() + '\"').toStdString().c_str());

        hyprpaperConfig.open(QIODevice::WriteOnly);
        hyprpaperConfig.write(("preload = " + wallpaper->getFilePath() + '\n').toStdString().c_str());
        hyprpaperConfig.write(("wallpaper = , " + wallpaper->getFilePath()).toStdString().c_str());
        hyprpaperConfig.close();

        logInfo("Wallpaper \"{}\" set", wallpaperName.toStdString());
        sendStatus("Wallpaper \"{}\" set", wallpaperName.toStdString());
    }

    inline void deleteWallpaper(const QString& wallpaperName) {
        const auto wallpaper = Wallpapers::getWallpaper(wallpaperName);

        if(!wallpaper) {
            logError("Wallpaper \"{}\" not found", wallpaperName.toStdString());
            sendStatus("Wallpaper \"{}\" not found", wallpaperName.toStdString());
            return;
        }

        Wallpapers::deleteWallpaper(*wallpaper);

        logInfo("Wallpaper \"{}\" deleted", wallpaperName.toStdString());
        sendStatus("Wallpaper \"{}\" deleted", wallpaperName.toStdString());
    }
}
