#pragma once

#include <QJsonArray>
#include "Wallpaper.hpp"

class Wallpapers {
public:
    static void load();

    static const QVector<Wallpaper>& getWallpapers();

    static const Wallpaper& getWallpaper(const QString& name);

    static QJsonArray toJson();

private:
    static const inline QJsonObject defaultWallpaperData = {
        {"description", "Wallpaper description."},
        {"tags", QJsonArray{"General"}}
    };
    static QVector<Wallpaper> wallpapers;
};
