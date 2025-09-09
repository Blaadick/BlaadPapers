#pragma once

#include <QFile>
#include <QStandardPaths>
#include <QString>

inline void applyWallpaper(const QString& picturePath) {
    //TODO Move some variables to Config.cpp
    //TODO Remove when implement own renderer

    QFile hyprpaperConfig(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/hypr/hyprpaper.conf");
    QString blaadpapersConfigPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/blaadpapers/");

    system("hyprctl -q hyprpaper unload all");
    system(("hyprctl -q hyprpaper preload \"" + picturePath + '\"').toStdString().c_str());
    system(("hyprctl -q hyprpaper wallpaper \", " + picturePath + '\"').toStdString().c_str());
    system(("bash " + blaadpapersConfigPath + "post_set.sh \"" + picturePath + '\"').toStdString().c_str());

    hyprpaperConfig.open(QIODevice::WriteOnly);
    hyprpaperConfig.write(("preload = " + picturePath + '\n').toStdString().c_str());
    hyprpaperConfig.write(("wallpaper = , " + picturePath).toStdString().c_str());
    hyprpaperConfig.close();
}
