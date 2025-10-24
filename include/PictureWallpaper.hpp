// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Wallpaper.hpp"

class PictureWallpaper : public Wallpaper {
public:
    PictureWallpaper(const QString& id, const QString& filePath, const QJsonObject& data);
};
