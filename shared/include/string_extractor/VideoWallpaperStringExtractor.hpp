// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "data/VideoWallpaper.hpp"
#include "string_extractor/TypedWallpaperStringExtractor.hpp"

class VideoWallpaperStringExtractor final : public TypedWallpaperStringExtractor<VideoWallpaper> {
public:
    std::string toStringTyped(const VideoWallpaper& wallpaper) const override;
};
