// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "string_extractor/TypedWallpaperStringExtractor.hpp"

template<std::derived_from<Wallpaper> T>
std::string TypedWallpaperStringExtractor<T>::toString(const Wallpaper& wallpaper) const {
    return toStringTyped(static_cast<const T&>(wallpaper));
}
