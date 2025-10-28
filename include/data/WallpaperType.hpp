// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QMap>
#include <QString>

enum class WallpaperType {
    Picture,
    Video,
    Scene
};

inline QString toString(const WallpaperType& type) {
    static QMap<WallpaperType, QString> strings{
        {WallpaperType::Picture, "Picture"},
        {WallpaperType::Video, "Video"},
        {WallpaperType::Scene, "Scene"}
    };

    return strings[type];
}
