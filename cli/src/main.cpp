// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <vips/vips8>
#include "Config.hpp"
#include "DefaultWallpaper.hpp"
#include "OptionExecutor.hpp"
#include "PostSetScript.hpp"
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "logger/CliLogger.hpp"

int main(const int argc, const char** argv) {
    vips_init(argv[0]);
    vips_cache_set_max(0);

    DefaultWallpaper::createIfNotExists();
    PostSetScript::createIfNotExists();

    auto logger = std::make_shared<util::CliLogger>();
    auto config = std::make_shared<Config>(logger);
    config->load();

    auto wallpapers = std::make_shared<Wallpapers>();
    auto wallpaperLoader = std::make_shared<WallpaperLoader>(wallpapers, config, logger);
    wallpaperLoader->loadWallpapers();
    wallpapers->sortByName();

    auto optionExecutor = std::make_shared<OptionExecutor>(wallpaperLoader, wallpapers, logger);
    const auto returnVal = optionExecutor->execute(argc, argv);

    vips_shutdown();
    return returnVal;
}
