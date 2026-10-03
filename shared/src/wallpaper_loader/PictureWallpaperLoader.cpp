// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper_loader/PictureWallpaperLoader.hpp"

#include <algorithm>
#include "data/PictureWallpaper.hpp"
#include "util/ImageUtils.hpp"

namespace fs = std::filesystem;

PictureWallpaperLoader::PictureWallpaperLoader(sptr<Config> config) : WallpaperLoader(
    std::move(config),
    {&file::png, &file::jpeg, &file::svg, &file::webp, &file::bmp, &file::avif, &file::tiff, &file::heif}
) {}

auto PictureWallpaperLoader::installWallpaper(
    const std::filesystem::path& filePath,
    std::optional<WallpaperData> wallpaperData
) const -> std::expected<sptr<Wallpaper>, std::string> {
    auto wallpaperId = filePath.stem().string();
    auto wallpaperDirPath = config->getWallpapersDirPath() / wallpaperId;
    auto wallpaperFilePath = wallpaperDirPath / ("wallpaper" + filePath.extension().string());
    auto wallpaperDataFilePath = wallpaperDirPath / "data.json";

    if(!wallpaperData) {
        wallpaperData = getDefaultWallpaperData(wallpaperId);
    }

    if(fs::exists(wallpaperDirPath)) {
        return std::unexpected("Wallpaper with same id already exists");
    }

    if(!fs::create_directory(wallpaperDirPath)) {
        return std::unexpected("Failed to create wallpaper directory");
    }

    if(!fs::copy_file(filePath, wallpaperFilePath)) {
        fs::remove_all(wallpaperDirPath);
        return std::unexpected("Failed to copy wallpaper file");
    }

    if(!saveWallpaperData(wallpaperDataFilePath, *wallpaperData)) {
        fs::remove_all(wallpaperDirPath);
        return std::unexpected("Failed to create wallpaper data");
    }

    auto pictureSize = getPictureResolutionData(wallpaperFilePath);
    if(!pictureSize.has_value()) {
        return std::unexpected("Failed to extract picture data");
    }

    return std::make_shared<PictureWallpaper>(
        wallpaperId,
        wallpaperFilePath,
        wallpaperDirPath,
        wallpaperData->name,
        *pictureSize,
        wallpaperData->source,
        wallpaperData->tags
    );
}

auto PictureWallpaperLoader::loadWallpaper(const std::filesystem::path& wallpaperFilePath) const -> std::expected<sptr<Wallpaper>, std::string> {
    auto wallpaperDirPath = wallpaperFilePath.parent_path();
    auto wallpaperDataFilePath = wallpaperDirPath / "data.json";
    auto wallpaperId = wallpaperDirPath.stem().string();

    auto wallpaperData = loadWallpaperData(wallpaperDataFilePath);;
    if(!wallpaperData.has_value()) {
        return std::unexpected("Failed to load wallpaper data");
    }

    auto pictureSize = getPictureResolutionData(wallpaperFilePath);
    if(!pictureSize.has_value()) {
        return std::unexpected("Failed to extract picture data");
    }

    return std::make_shared<PictureWallpaper>(
        wallpaperId,
        wallpaperFilePath,
        wallpaperDirPath,
        wallpaperData->name,
        *pictureSize,
        wallpaperData->source,
        wallpaperData->tags
    );
}
