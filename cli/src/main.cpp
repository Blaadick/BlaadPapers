// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include <vips/vips8>
#include "Config.hpp"
#include "DefaultWallpaper.hpp"
#include "OptionExecutor.hpp"
#include "PostSetScript.hpp"
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "deeplink_handler/ApplyHandler.hpp"
#include "logger/CliLogger.hpp"
#include "option/AddOption.hpp"
#include "option/ApplyOption.hpp"
#include "option/CountOption.hpp"
#include "option/InfoOption.hpp"
#include "option/ListOption.hpp"
#include "option/RemoveOption.hpp"
#include "option/RunRendererOption.hpp"
#include "option/ShuffleOption.hpp"
#include "option/VersionOption.hpp"

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

    auto optionExecutor = std::make_shared<OptionExecutor>(logger);
    optionExecutor->addOption("add", std::make_unique<AddOption>(wallpaperLoader, config, logger));
    optionExecutor->addOption("apply", std::make_unique<ApplyOption>(wallpapers, logger));
    optionExecutor->addOption("count", std::make_unique<CountOption>(wallpapers, logger));
    optionExecutor->addOption("info", std::make_unique<InfoOption>(wallpapers, logger));
    optionExecutor->addOption("list", std::make_unique<ListOption>(wallpapers, logger));
    optionExecutor->addOption("remove", std::make_unique<RemoveOption>(wallpapers, logger));
    optionExecutor->addOption("run-renderer", std::make_unique<RunRendererOption>(wallpapers, logger));
    optionExecutor->addOption("shuffle", std::make_unique<ShuffleOption>(wallpapers, logger));
    optionExecutor->addOption("version", std::make_unique<VersionOption>(logger));

    optionExecutor->addHandler("apply", std::make_unique<ApplyHandler>(wallpapers, logger));

    const auto returnVal = optionExecutor->execute(argc, argv);

    vips_shutdown();
    return returnVal;
}
