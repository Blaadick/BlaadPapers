// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

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

    virtual auto loadWallpaper(const std::filesystem::path& wallpaperFilePath) const -> uptr<Wallpaper> = 0;

    auto getSupportedFileTypes() const -> const std::unordered_set<const file::FileType*>&;

    auto isSupported(const std::filesystem::path& wallpaperFilePath) const -> bool;

    auto loadWallpaperData(const std::filesystem::path& wallpaperDataFilePath) const -> std::optional<WallpaperData>;

    void saveWallpaperData(const std::filesystem::path& wallpaperDataFilePath, const WallpaperData& wallpaperData) const;

protected:
    const std::unordered_set<const file::FileType*> supportedFileTypes;
    sptr<util::Logger> logger;
};
