// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QFileInfo>
#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "util/Pointers.hpp"

class WallpaperLoader {
public:
    static void loadWallpapers();

private:
    static QJsonObject readWallpaperData(const QString& filePath, const QString& wallpaperId);

    static uptr<PictureWallpaper> loadPictureWallpaper(const QFileInfo& fileInfo, const QJsonObject& data);

    static uptr<VideoWallpaper> loadVideoWallpaper(const QFileInfo& fileInfo, const QJsonObject& data);

    /**
    * We are not in DOS time! I turn it off if it is too annoying.
    */
    static void jpegUnifier();
};
