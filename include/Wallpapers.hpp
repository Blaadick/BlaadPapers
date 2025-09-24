#pragma once

#include <QJsonArray>
#include "Wallpaper.hpp"

class Wallpapers {
public:
    static void load();

    static const QVector<Wallpaper>& getWallpapers();

    static const Wallpaper* getWallpaper(const QString& name);

    static void deleteWallpaper(const Wallpaper& wallpaper);

    static const QMap<QString, int>& getUniqueTags();

    static QJsonArray toJson();

private:
    static const inline QJsonObject defaultWallpaperData = {
        {"tags", QJsonArray{"General"}}
    };
    static QVector<Wallpaper> wallpapers;
    static QMap<QString, int> uniqueTags;
};
