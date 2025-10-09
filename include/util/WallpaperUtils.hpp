// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QFile>
#include <QStandardPaths>
#include <QString>
#include "Config.hpp"
#include "model/ConfigModel.hpp"
#include "util/Loggers.hpp"

namespace util {
    inline void applyWallpaper(const QString& wallpaperId) {
        //TODO Remove when implement own renderer

        const Wallpaper* wallpaper = Wallpapers::getWallpaper(wallpaperId);

        if(!wallpaper) {
            logError("Wallpaper {} not found", wallpaperId.toStdString());
            sendStatus("Wallpaper {} not found", wallpaperId.toStdString());
            return;
        }

        const auto de = qgetenv("XDG_SESSION_DESKTOP").toStdString();
        if(de == "Hyprland") {
            QFile hyprpaperConfig(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/hypr/hyprpaper.conf");

            system("hyprctl -q hyprpaper unload all");
            system(("hyprctl -q hyprpaper preload \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
            system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper->getFilePath() + '\"').toStdString().c_str());

            hyprpaperConfig.open(QIODevice::WriteOnly);
            hyprpaperConfig.write(("preload = " + wallpaper->getFilePath() + '\n').toStdString().c_str());
            hyprpaperConfig.write(("wallpaper = , " + wallpaper->getFilePath()).toStdString().c_str());
            hyprpaperConfig.close();
        } else if(de == "KDE") {
            system(("plasma-apply-wallpaperimage \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
        } else if(de == "gnome" || de == "ubuntu") {
            system(("gsettings set org.gnome.desktop.background picture-uri \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
            system(("gsettings set org.gnome.desktop.background picture-uri-dark \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
        } else {
            logError("Desktop environment \"{}\" unsupported", de);
            sendStatus("Desktop environment \"{}\" unsupported", de);
            return;
        }

        ConfigModel::inst().setStatusBarVisible(!ConfigModel::inst().getStatusBarVisible());

        logInfo("Wallpaper {} set", wallpaperId.toStdString());
        sendStatus("Wallpaper {} set", wallpaperId.toStdString());
        system(("bash " + Config::getPostSetScriptFilePath() + " \"" + wallpaper->getName() + '\"' + " \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
    }

    inline void deleteWallpaper(const QString& wallpaperId) {
        const auto wallpaper = Wallpapers::getWallpaper(wallpaperId);

        if(!wallpaper) {
            logError("Wallpaper {} not found", wallpaperId.toStdString());
            sendStatus("Wallpaper {} not found", wallpaperId.toStdString());
            return;
        }

        Wallpapers::deleteWallpaper(*wallpaper);

        logInfo("Wallpaper {} deleted", wallpaperId.toStdString());
        sendStatus("Wallpaper {} deleted", wallpaperId.toStdString());
    }
}
