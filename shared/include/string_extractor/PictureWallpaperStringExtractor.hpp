// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "data/PictureWallpaper.hpp"
#include "string_extractor/TypedWallpaperStringExtractor.hpp"

class PictureWallpaperStringExtractor final : public TypedWallpaperStringExtractor<PictureWallpaper> {
public:
    std::string toStringTyped(const PictureWallpaper& wallpaper) const override;
};
