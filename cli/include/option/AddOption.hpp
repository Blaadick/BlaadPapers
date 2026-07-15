// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"

class AddOption final : public Option {
public:
    AddOption(sptr<WallpaperLoader> wallpaperLoader, sptr<Config> config, sptr<util::Logger> logger);

    int execute(const std::vector<std::string_view>& arguments) override;

private:
    sptr<WallpaperLoader> wallpaperLoader;
    sptr<Config> config;
    sptr<util::Logger> logger;
};
