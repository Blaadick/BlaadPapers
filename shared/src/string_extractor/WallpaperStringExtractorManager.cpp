// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "string_extractor/WallpaperStringExtractorManager.hpp"

std::string WallpaperStringExtractorManager::toString(const Wallpaper& wallpaper) const {
    const auto it = stringExtractors.find(typeid(wallpaper));

    if(it == stringExtractors.end()) {
        throw std::runtime_error(std::format("String extractor not found for \"{}\" type", typeid(wallpaper).name()));
    }

    return it->second->toString(wallpaper);
}

template<std::derived_from<Wallpaper> T>
void WallpaperStringExtractorManager::addStringExtractor(uptr<WallpaperStringExtractor> stringExtractor) {
    stringExtractors.emplace(typeid(T), std::move(stringExtractor));
}
