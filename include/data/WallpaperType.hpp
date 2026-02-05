// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QMap>
#include <QString>

enum class WallpaperType {
    Picture,
    Video
};

inline QString toString(const WallpaperType& type) {
    static QMap<WallpaperType, QString> strings{
        {WallpaperType::Picture, "Picture"},
        {WallpaperType::Video, "Video"}
    };

    return strings[type];
}
