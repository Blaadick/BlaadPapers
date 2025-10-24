// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Wallpaper.hpp"

class SceneWallpaper : public Wallpaper {
public:
    SceneWallpaper(const QString& id, const QString& filePath, const QJsonObject& data);
};
