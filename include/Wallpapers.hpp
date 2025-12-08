// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QJsonArray>
#include "data/Wallpaper.hpp"

class Wallpapers {
public:
    static void load();

    [[nodiscard]]
    static const QVector<Wallpaper>& getWallpapers();

    [[nodiscard]]
    static std::optional<std::reference_wrapper<const Wallpaper>> getWallpaper(const QString& wallpaperId);

    [[nodiscard]]
    static int count();

    static void deleteWallpaper(const Wallpaper& wallpaper);

    static void deleteWallpaper(const QString& wallpaperId);

    static void applyWallpaper(const Wallpaper& wallpaper);

    static void applyWallpaper(const QString& wallpaperId);

    [[nodiscard]]
    static QJsonArray toJson();

private:
    static QVector<Wallpaper> wallpapers;
};
