// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "data/Wallpaper.hpp"
#include "util/Pointers.hpp"

class TagRepository final {
public:
    auto getTaggedWallpaper(const std::string& tag) const -> std::unordered_set<sptr<Wallpaper>>;

    auto getWallpaperTags(sptr<Wallpaper> wallpaper) const -> std::unordered_set<std::string>;

    void tagWallpaper(sptr<Wallpaper> wallpaper, const std::string& tag);

    void untagWallpaper(sptr<Wallpaper> wallpaper, const std::string& tag);

    void toString() const;

private:
    std::unordered_map<std::string, std::unordered_set<sptr<Wallpaper>>> tagsToWallpapers;
    std::unordered_map<sptr<Wallpaper>, std::unordered_set<std::string>> wallpapersToTags;
};
