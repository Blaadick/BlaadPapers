// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include <filesystem>

class DefaultWallpaper {
public:
    static std::filesystem::path defaultWallpaperPath();

    static void createIfNotExists();
};
