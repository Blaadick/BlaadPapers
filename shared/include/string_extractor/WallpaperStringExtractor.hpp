// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include "data/Wallpaper.hpp"

class WallpaperStringExtractor {
public:
    virtual ~WallpaperStringExtractor() = default;

    virtual std::string toString(const Wallpaper& wallpaper) const = 0;
};
