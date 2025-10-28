// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QMap>
#include <QString>

enum class WallpaperType {
    PICTURE,
    VIDEO,
    SCENE
};

inline QString toString(const WallpaperType& type) {
    static QMap<WallpaperType, QString> strings{
        {WallpaperType::PICTURE, "Picture"},
        {WallpaperType::VIDEO, "Video"},
        {WallpaperType::SCENE, "Scene"}
    };

    return strings[type];
}
