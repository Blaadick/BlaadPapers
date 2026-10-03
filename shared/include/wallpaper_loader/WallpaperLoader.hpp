// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <unordered_set>
#include "config/Config.hpp"
#include "data/Wallpaper.hpp"
#include "data/WallpaperData.hpp"
#include "file_processing/FileType.hpp"
#include "util/Pointers.hpp"

class WallpaperLoader {
public:
    explicit WallpaperLoader(sptr<Config> config, std::unordered_set<const file::FileType*> supportedFileTypes);

    virtual ~WallpaperLoader() = default;

    virtual auto installWallpaper(
        const std::filesystem::path& filePath,
        std::optional<WallpaperData> wallpaperData
    ) const -> std::expected<sptr<Wallpaper>, std::string> = 0;

    virtual auto loadWallpaper(const std::filesystem::path& wallpaperFilePath) const -> std::expected<sptr<Wallpaper>, std::string> = 0;

    auto getSupportedFileTypes() const noexcept -> const std::unordered_set<const file::FileType*>&;

    auto isSupported(const std::filesystem::path& filePath) const -> bool;

    auto getDefaultWallpaperData(const std::string& wallpaperId) const noexcept -> WallpaperData;

    auto loadWallpaperData(const std::filesystem::path& wallpaperDataFilePath) const -> std::optional<WallpaperData>;

    bool saveWallpaperData(const std::filesystem::path& wallpaperDataFilePath, const WallpaperData& wallpaperData) const;

protected:
    sptr<Config> config;

    const std::unordered_set<const file::FileType*> supportedFileTypes;
};
