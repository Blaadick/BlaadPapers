// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "data/Wallpaper.hpp"
#include "util/Pointers.hpp"

class WallpaperRepository {
public:
    auto get(int index) const -> sptr<Wallpaper>;

    auto get(std::string_view id) const -> sptr<Wallpaper>;

    auto shuffle(
        std::vector<std::string> includeTags = {},
        std::vector<std::string> excludeTags = {}
    ) const -> std::optional<sptr<Wallpaper>>;

    void add(sptr<Wallpaper> wallpaper);

    auto apply(std::string_view id) const -> bool;

    auto apply(const Wallpaper& wallpaper) const -> bool;

    auto remove(std::string_view id) -> bool;

    void sortByName();

    void clear();

    auto count() const -> int;

    auto begin() const -> std::vector<sptr<Wallpaper>>::const_iterator;

    auto end() const -> std::vector<sptr<Wallpaper>>::const_iterator;

private:
    std::vector<sptr<Wallpaper>> wallpapers;
};
