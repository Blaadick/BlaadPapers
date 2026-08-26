// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <unordered_set>
#include "data/Wallpaper.hpp"
#include "data/WallpaperData.hpp"
#include "file_processing/FileType.hpp"
#include "logger/Logger.hpp"
#include "util/Pointers.hpp"

class WallpaperLoader {
public:
    WallpaperLoader(std::unordered_set<const file::FileType*> supportedFileTypes, sptr<util::Logger> logger);

    virtual ~WallpaperLoader() = default;

    const std::unordered_set<const file::FileType*>& getSupportedFileTypes() const;

    bool isSupported(const std::filesystem::path& wallpaperFilePath) const;

    std::optional<WallpaperData> loadWallpaperData(const std::filesystem::path& wallpaperDataFilePath) const;

    void saveWallpaperData(const std::filesystem::path& wallpaperDataFilePath, const WallpaperData& wallpaperData) const;

    virtual uptr<Wallpaper> loadWallpaper(const std::filesystem::path& wallpaperFilePath) const = 0;

protected:
    const std::unordered_set<const file::FileType*> supportedFileTypes;
    sptr<util::Logger> logger;
};
