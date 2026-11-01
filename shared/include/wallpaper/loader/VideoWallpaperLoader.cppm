// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:VideoWallpaperLoader;

import std;
import :Config;
import :Pointers;
import :Wallpaper;
import :WallpaperData;
import :WallpaperLoader;

export class VideoWallpaperLoader final : public WallpaperLoader {
public:
    explicit VideoWallpaperLoader(sptr<Config> config);

    auto installWallpaper(
        const std::filesystem::path& filePath,
        std::optional<WallpaperData> wallpaperData
    ) const -> std::expected<sptr<Wallpaper>, std::string> override;

    auto loadWallpaper(std::filesystem::path wallpaperFilePath) const -> std::expected<sptr<Wallpaper>, std::string> override;
};
