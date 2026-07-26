// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "string_extractor/VideoWallpaperStringExtractor.hpp"

std::string VideoWallpaperStringExtractor::toStringTyped(const VideoWallpaper& wallpaper) const {
    return std::format(
        "{}\n"
        "    Name: {}\n"
        "    FilePath: {}\n"
        "    Resolution: {}\n"
        "    FrameRate: {}\n"
        "    Source: {}\n"
        "    Tags: {}\n"
        "    Type: {}\n",
        wallpaper.getId(),
        wallpaper.getName(),
        wallpaper.getFilePath().string(),
        wallpaper.getResolution().toString(),
        wallpaper.getFrameRate(),
        wallpaper.getSource(),
        wallpaper.getTags(),
        "Video"
    );
}
