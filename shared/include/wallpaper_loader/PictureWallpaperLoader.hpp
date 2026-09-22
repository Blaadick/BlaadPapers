// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "wallpaper_loader/WallpaperLoader.hpp"

class PictureWallpaperLoader final : public WallpaperLoader {
public:
    PictureWallpaperLoader();

    auto installWallpaper(
        const std::filesystem::path& filePath,
        const std::filesystem::path& destinationFolderPath,
        std::optional<WallpaperData> wallpaperData
    ) const -> std::expected<uptr<Wallpaper>, std::string> override;

    auto loadWallpaper(const std::filesystem::path& wallpaperFilePath) const -> std::expected<uptr<Wallpaper>, std::string> override;
};
