// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include "data/Wallpaper.hpp"
#include "data/WallpaperData.hpp"
#include "logger/Logger.hpp"
#include "util/Pointers.hpp"

class WallpaperLoader {
public:
    explicit WallpaperLoader(sptr<util::Logger> logger);

    virtual ~WallpaperLoader() = default;

    virtual bool isSupported(const std::filesystem::path& wallpaperFilePath) const = 0;

    virtual uptr<Wallpaper> loadWallpaper(const std::filesystem::path& wallpaperFilePath) const = 0;

    std::optional<WallpaperData> loadWallpaperData(const std::filesystem::path& wallpaperDataFilePath) const;

    void saveWallpaperData(const std::filesystem::path& wallpaperDataFilePath, const WallpaperData& wallpaperData) const;

protected:
    sptr<util::Logger> logger;
};
