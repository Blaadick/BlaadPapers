// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "SceneWallpaper.hpp"

SceneWallpaper::SceneWallpaper(
    const QString& id,
    const QString& filePath,
    const QJsonObject& data
) : Wallpaper(id, filePath, data) {}
