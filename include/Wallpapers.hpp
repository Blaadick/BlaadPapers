#pragma once

#include <QJsonArray>
#include "Wallpaper.hpp"

class Wallpapers {
public:
    static void load();

    static QVector<Wallpaper> getWallpapers();

    static QJsonArray toJson();

private:
    static const inline QJsonObject defaultWallpaperData = {
        {"description", "Wallpaper description."},
        {"tags", QJsonArray{"General"}}
    };
    static QVector<Wallpaper> wallpapers;
};
