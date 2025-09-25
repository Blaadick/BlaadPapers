#pragma once

#include <QFile>
#include <QStandardPaths>
#include <QString>
#include "Config.hpp"
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

        QFile hyprpaperConfig(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/hypr/hyprpaper.conf");

        logInfo("Wallpaper {} set", wallpaperId.toStdString());
        sendStatus("Wallpaper {} set", wallpaperId.toStdString());
        system(("bash " + Config::getPostSetScriptPath() + " \"" + wallpaper->getName() + '\"' + " \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());

        const auto desktopEnvironment = std::string(getenv("XDG_SESSION_DESKTOP"));
        if(desktopEnvironment == "Hyprland") {
            system("hyprctl -q hyprpaper unload all");
            system(("hyprctl -q hyprpaper preload \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
            system(("hyprctl -q hyprpaper wallpaper \", " + wallpaper->getFilePath() + '\"').toStdString().c_str());

            hyprpaperConfig.open(QIODevice::WriteOnly);
            hyprpaperConfig.write(("preload = " + wallpaper->getFilePath() + '\n').toStdString().c_str());
            hyprpaperConfig.write(("wallpaper = , " + wallpaper->getFilePath()).toStdString().c_str());
            hyprpaperConfig.close();
        } else if(desktopEnvironment == "KDE") {
            system(("plasma-apply-wallpaperimage \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
        } else if(desktopEnvironment == "gnome") {
            system(("gsettings set org.gnome.desktop.background picture-uri \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
            system(("gsettings set org.gnome.desktop.background picture-uri-dark \"" + wallpaper->getFilePath() + '\"').toStdString().c_str());
        } else {
            logError("Desktop environment \"{}\" unsupported", desktopEnvironment);
            sendStatus("Desktop environment \"{}\" unsupported", desktopEnvironment);
        }
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
