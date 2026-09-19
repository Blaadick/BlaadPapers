// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include "data/Wallpaper.hpp"
#include "util/Pointers.hpp"

class WallpaperRepository {
public:
    auto get(int index) const -> Wallpaper*;

    auto get(std::string_view id) const -> Wallpaper*;

    auto shuffle(
        std::optional<std::vector<std::string>> includeTags = std::nullopt,
        std::optional<std::vector<std::string>> excludeTags = std::nullopt
    ) const -> Wallpaper*;

    void add(uptr<Wallpaper> wallpaper);

    auto apply(std::string_view id) const -> bool;

    auto apply(const Wallpaper& wallpaper) const -> bool;

    auto remove(std::string_view id) -> bool;

    void sortByName();

    void clear();

    auto count() const -> int;

    auto begin() const -> std::vector<uptr<Wallpaper>>::const_iterator;

    auto end() const -> std::vector<uptr<Wallpaper>>::const_iterator;

    auto operator[](int index) const -> Wallpaper*;

    auto operator[](std::string_view id) const -> Wallpaper*;

private:
    std::vector<uptr<Wallpaper>> wallpapers;
};
