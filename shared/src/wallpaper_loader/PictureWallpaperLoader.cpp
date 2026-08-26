// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "wallpaper_loader/PictureWallpaperLoader.hpp"

#include <algorithm>
#include "data/PictureWallpaper.hpp"
#include "util/ImageUtils.hpp"

namespace fs = std::filesystem;

PictureWallpaperLoader::PictureWallpaperLoader(sptr<util::Logger> logger) : WallpaperLoader(
    {&file::png, &file::jpeg, &file::svg, &file::webp, &file::bmp, &file::avif, &file::tiff, &file::heif},
    std::move(logger)
) {}

uptr<Wallpaper> PictureWallpaperLoader::loadWallpaper(const std::filesystem::path& wallpaperFilePath) const {
    auto wallpaperDirPath = wallpaperFilePath.parent_path();
    auto wallpaperDataFilePath = wallpaperDirPath / "data.json";
    auto wallpaperId = wallpaperDirPath.stem().string();

    auto wallpaperData = loadWallpaperData(wallpaperDataFilePath);;
    if(!wallpaperData.has_value()) {
        return nullptr;
    }

    return std::make_unique<PictureWallpaper>(
        wallpaperId,
        wallpaperFilePath,
        wallpaperDirPath,
        wallpaperData->name,
        getPictureResolutionData(wallpaperFilePath),
        wallpaperData->source,
        wallpaperData->tags
    );
}
