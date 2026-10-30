// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <string>
#include "ShellExecutor.hpp"
#include "config/Config.hpp"
#include "util/Pointers.hpp"
#include "wallpaper/Wallpaper.hpp"

class WallpaperRepository final {
public:
    WallpaperRepository(sptr<Config> config, sptr<ShellExecutor> shellExecutor);

    auto get(int index) const noexcept -> sptr<Wallpaper>;

    auto get(std::string_view id) const noexcept -> sptr<Wallpaper>;

    auto shuffle(
        std::vector<std::string> includeTags = {},
        std::vector<std::string> excludeTags = {}
    ) const -> std::optional<sptr<Wallpaper>>;

    void add(sptr<Wallpaper> wallpaper);

    auto apply(std::string_view id) const -> std::expected<void, std::string>;

    auto apply(const Wallpaper& wallpaper) const -> std::expected<void, std::string>;

    auto remove(std::string_view id) -> std::expected<void, std::string>;

    void sortByName() noexcept;

    void clear() noexcept;

    auto count() const noexcept -> int;

    auto begin() const noexcept -> std::vector<sptr<Wallpaper>>::const_iterator;

    auto end() const noexcept -> std::vector<sptr<Wallpaper>>::const_iterator;

private:
    sptr<Config> config;
    sptr<ShellExecutor> shellExecutor;

    std::vector<sptr<Wallpaper>> wallpapers;
};
