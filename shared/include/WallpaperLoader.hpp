// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "util/Pointers.hpp"

class WallpaperLoader {
public:
    /**
     * Vips should be initialized before run!
     */
    static void loadWallpapers();

    static bool addWallpaper(
        const std::filesystem::path& wallpaperFilePath,
        const std::filesystem::path& destinationFolderPath
    );

    static void addWallpapers(
        const std::vector<std::filesystem::path>& paths,
        const std::filesystem::path& destinationFolderPath
    );

private:
    static nlohmann::json readWallpaperData(const std::filesystem::path& wallpaperDataPath);

    static uptr<PictureWallpaper> loadPictureWallpaper(
        const std::string& wallpaperId,
        const std::filesystem::path& filePath,
        const nlohmann::json& data
    );

    static uptr<VideoWallpaper> loadVideoWallpaper(
        const std::string& wallpaperId,
        const std::filesystem::path& filePath,
        const nlohmann::json& data
    );

    /**
    * We are not in DOS time! I turn it off if it is too annoying.
    */
    static void formatUnifier();
};
