// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper_loader/VideoWallpaperLoader.hpp"

#include <algorithm>
#include "data/VideoWallpaper.hpp"
#include "util/Ffmpeg.hpp"

namespace fs = std::filesystem;

VideoWallpaperLoader::VideoWallpaperLoader(sptr<Config> config) : WallpaperLoader(
    std::move(config),
    {&file::mp4, &file::webm, &file::mkv}
) {}

auto VideoWallpaperLoader::installWallpaper(
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
        return std::unexpected("Failed to save wallpaper data");
    }

    auto videoData = getVideoData(wallpaperFilePath);
    if(!videoData.has_value()) {
        return std::unexpected("Failed to decode video data");
    }

    return std::make_shared<VideoWallpaper>(
        wallpaperId,
        wallpaperFilePath,
        wallpaperDirPath,
        wallpaperData->name,
        videoData->resolution,
        videoData->frameRate,
        wallpaperData->source,
        wallpaperData->tags
    );
}

auto VideoWallpaperLoader::loadWallpaper(const std::filesystem::path& wallpaperFilePath) const -> std::expected<sptr<Wallpaper>, std::string> {
    auto wallpaperDirPath = wallpaperFilePath.parent_path();
    auto wallpaperDataFilePath = wallpaperDirPath / "data.json";
    auto wallpaperId = wallpaperDirPath.stem().string();

    auto wallpaperData = loadWallpaperData(wallpaperDataFilePath);;
    if(!wallpaperData.has_value()) {
        return std::unexpected("Failed to load wallpaper data");
    }

    auto videoData = getVideoData(wallpaperFilePath);
    if(!videoData.has_value()) {
        return std::unexpected("Failed to decode video data");
    }

    return std::make_shared<VideoWallpaper>(
        wallpaperId,
        wallpaperFilePath,
        wallpaperDirPath,
        wallpaperData->name,
        videoData->resolution,
        videoData->frameRate,
        wallpaperData->source,
        wallpaperData->tags
    );
}
