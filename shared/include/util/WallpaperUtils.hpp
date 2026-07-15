// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "data/VideoWallpaper.hpp"
#include "data/Wallpaper.hpp"

namespace util {
    const std::unordered_set<std::string> supportedPictureFormats = {
        ".png",
        ".apng",
        ".jpeg", ".jpg",
        ".webp",
        ".bmp",
        ".avif",
        ".tiff", ".tif",
        ".heic", ".heif"
    };

    const std::unordered_set<std::string> supportedVideoFormats = {
        ".mp4",
        ".webm",
        ".heis"
    };

    inline bool isSupportedPicture(const std::filesystem::path& path) {
        return supportedPictureFormats.contains(path.extension());
    }

    inline bool isSupportedVideo(const std::filesystem::path& path) {
        return supportedVideoFormats.contains(path.extension());
    }

    inline bool isSupported(const std::filesystem::path& path) {
        return isSupportedPicture(path) || isSupportedVideo(path);
    }
}
