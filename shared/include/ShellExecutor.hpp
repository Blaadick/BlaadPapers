// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <string_view>
#include "logger/Logger.hpp"
#include "wallpaper/Wallpaper.hpp"

class ShellExecutor final {
public:
    auto execute(const std::string& command, const std::vector<std::string>& customEnv = {}) const -> std::expected<int, std::string>;

    auto executeWithWallpaperData(const std::string& command, const Wallpaper& wallpaper) const -> std::expected<int, std::string>;
};
