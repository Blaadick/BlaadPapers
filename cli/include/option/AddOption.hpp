// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../../../shared/include/wallpaper_loader/WallpaperLoaderManager.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"

class AddOption final : public Option {
public:
    AddOption(sptr<WallpaperLoaderManager> wallpaperLoader, sptr<Config> config, sptr<util::Logger> logger);

    [[nodiscard]]
    std::vector<std::string_view> getUsageStrings() const override;

    [[nodiscard]]
    int execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) override;

private:
    sptr<WallpaperLoaderManager> wallpaperLoader;
    sptr<Config> config;
    sptr<util::Logger> logger;
};
