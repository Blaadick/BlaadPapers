// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeindex>
#include <unordered_map>
#include "data/Wallpaper.hpp"
#include "string_extractor/WallpaperStringExtractor.hpp"
#include "util/Pointers.hpp"

class WallpaperStringExtractorManager final {
public:
    std::string toString(const Wallpaper& wallpaper) const;

    template<std::derived_from<Wallpaper> T>
    void addStringExtractor(uptr<WallpaperStringExtractor> stringExtractor);

private:
    std::unordered_map<std::type_index, uptr<WallpaperStringExtractor>> stringExtractors;
};
