// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:DefaultWallpaper;

import std;

export class DefaultWallpaper final {
public:
    static auto defaultWallpaperFilePath() -> const std::filesystem::path&;

    static void create();
};
