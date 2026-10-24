// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>

class DefaultWallpaper final {
public:
    static auto defaultWallpaperFilePath() -> const std::filesystem::path&;

    static void create();
};
