// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonArray>
#include "Wallpaper.hpp"

class Wallpapers {
public:
    static void load();

    [[nodiscard]]
    static const QVector<Wallpaper>& getWallpapers();

    [[nodiscard]]
    static const Wallpaper* getWallpaper(const QString& id);

    static void deleteWallpaper(const Wallpaper& wallpaper);

    [[nodiscard]]
    static const QMap<QString, int>& getUniqueTags();

    [[nodiscard]]
    static QJsonArray toJson();

private:
    static QVector<Wallpaper> wallpapers;
    static QMap<QString, int> uniqueTags;
};
