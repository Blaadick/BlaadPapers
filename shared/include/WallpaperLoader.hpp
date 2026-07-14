// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>

#include "Config.hpp"
#include "Wallpapers.hpp"
#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "util/Pointers.hpp"

class WallpaperLoader {
public:
    WallpaperLoader(sptr<Wallpapers> wallpapers, sptr<Config> config, sptr<util::Logger> logger);

    /**
     * Vips should be initialized before run!
     */
    void loadWallpapers();

    bool addWallpaper(
        const std::filesystem::path& wallpaperFilePath,
        const std::filesystem::path& destinationFolderPath
    );

    void addWallpapers(
        const std::vector<std::filesystem::path>& paths,
        const std::filesystem::path& destinationFolderPath
    );

private:
    sptr<Wallpapers> wallpapers;
    sptr<Config> config;
    sptr<util::Logger> logger;

    nlohmann::json readWallpaperData(const std::filesystem::path& wallpaperDataPath);

    uptr<PictureWallpaper> loadPictureWallpaper(
        const std::string& wallpaperId,
        const std::filesystem::path& filePath,
        const nlohmann::json& data
    );

    uptr<VideoWallpaper> loadVideoWallpaper(
        const std::string& wallpaperId,
        const std::filesystem::path& filePath,
        const nlohmann::json& data
    );

    /**
    * We are not in DOS time! I turn it off if it is too annoying.
    */
    void formatUnifier();
};
