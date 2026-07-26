// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "data/Wallpaper.hpp"
#include "string_extractor/WallpaperStringExtractor.hpp"

template<std::derived_from<Wallpaper> T>
class TypedWallpaperStringExtractor : public WallpaperStringExtractor {
public:
    std::string toString(const Wallpaper& wallpaper) const override;

    virtual std::string toStringTyped(const T& wallpaper) const = 0;
};
