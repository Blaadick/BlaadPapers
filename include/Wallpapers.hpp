// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonArray>
#include "Wallpaper.hpp"

class Wallpapers {
public:
    static void load();

    static const QVector<Wallpaper>& getWallpapers();

    static const Wallpaper* getWallpaper(const QString& id);

    static void deleteWallpaper(const Wallpaper& wallpaper);

    static const QMap<QString, int>& getUniqueTags();

    static QJsonArray toJson();

private:
    static QVector<Wallpaper> wallpapers;
    static QMap<QString, int> uniqueTags;
};
