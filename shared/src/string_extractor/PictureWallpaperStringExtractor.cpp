// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "string_extractor/PictureWallpaperStringExtractor.hpp"

std::string PictureWallpaperStringExtractor::toStringTyped(const PictureWallpaper& wallpaper) const {
    return std::format(
        "{}\n"
        "    Name: {}\n"
        "    FilePath: {}\n"
        "    Resolution: {}\n"
        "    Source: {}\n"
        "    Tags: {}\n"
        "    Type: {}\n",
        wallpaper.getId(),
        wallpaper.getName(),
        wallpaper.getFilePath().string(),
        wallpaper.getResolution().toString(),
        wallpaper.getSource(),
        wallpaper.getTags(),
        "Picture"
    );
}
