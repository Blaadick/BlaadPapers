// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "wallpaper_loader/VideoWallpaperLoader.hpp"

#include <algorithm>
#include "data/VideoWallpaper.hpp"
#include "util/Ffmpeg.hpp"

namespace fs = std::filesystem;

VideoWallpaperLoader::VideoWallpaperLoader(sptr<util::Logger> logger) : WallpaperLoader(
    {&file::mp4, &file::webm, &file::mkv},
    std::move(logger)
) {}

uptr<Wallpaper> VideoWallpaperLoader::loadWallpaper(const std::filesystem::path& wallpaperFilePath) const {
    auto wallpaperDirPath = wallpaperFilePath.parent_path();
    auto wallpaperDataFilePath = wallpaperDirPath / "data.json";
    auto wallpaperId = wallpaperDirPath.stem().string();

    auto wallpaperData = loadWallpaperData(wallpaperDataFilePath);;
    if(!wallpaperData.has_value()) {
        return nullptr;
    }

    auto videoData = getVideoData(wallpaperFilePath);
    if(!videoData.has_value()) {
        logger->logWarning(std::format("Failed to decode video data from \"{}\"", wallpaperFilePath.string()));
        return nullptr;
    }

    return std::make_unique<VideoWallpaper>(
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
