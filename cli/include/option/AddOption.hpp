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

    [[nodiscard]]
    std::string getHelpMessage() const override;

    [[nodiscard]]
    int execute(const std::vector<std::string>& arguments) override;

private:
    sptr<WallpaperLoader> wallpaperLoader;
    sptr<Config> config;
    sptr<util::Logger> logger;
};
