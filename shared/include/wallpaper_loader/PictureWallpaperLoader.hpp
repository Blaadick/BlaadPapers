// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "logger/Logger.hpp"
#include "wallpaper_loader/WallpaperLoader.hpp"

class PictureWallpaperLoader final : public WallpaperLoader {
public:
    explicit PictureWallpaperLoader(sptr<util::Logger> logger);

    uptr<Wallpaper> loadWallpaper(const std::filesystem::path& wallpaperFilePath) const override;
};
