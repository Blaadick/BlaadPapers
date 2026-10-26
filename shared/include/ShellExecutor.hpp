// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>
#include "logger/Logger.hpp"
#include "util/Pointers.hpp"
#include "wallpaper/Wallpaper.hpp"

class ShellExecutor final {
public:
    explicit ShellExecutor(sptr<util::Logger> logger);

    auto execute(std::string_view command, std::vector<std::string> customEnv = {}) const -> int;

    auto executeWithWallpaperData(std::string_view command, const Wallpaper& wallpaper) const -> int;

private:
    sptr<util::Logger> logger;
};
