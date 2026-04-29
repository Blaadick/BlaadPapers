// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "util/Pointers.hpp"

class WallpaperLoader {
public:
    static void loadWallpapers();

private:
    static nlohmann::json readWallpaperData(const std::filesystem::path& wallpaperDataPath);

    static uptr<PictureWallpaper> loadPictureWallpaper(const std::filesystem::path& filePath, const nlohmann::json& data);

    static uptr<VideoWallpaper> loadVideoWallpaper(const std::filesystem::path& filePath, const nlohmann::json& data);

    /**
    * We are not in DOS time! I turn it off if it is too annoying.
    */
    static void jpegUnifier();
};
