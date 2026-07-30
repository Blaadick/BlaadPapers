// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "CliExecutor.hpp"
#include "Config.hpp"
#include "DefaultWallpaper.hpp"
#include "PostSetScript.hpp"
#include "Wallpapers.hpp"
#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "deeplink_handler/ApplyHandler.hpp"
#include "deeplink_handler/ShuffleHandler.hpp"
#include "flag/Flags.hpp"
#include "logger/CliLogger.hpp"
#include "option/AddOption.hpp"
#include "option/ApplyOption.hpp"
#include "option/CountOption.hpp"
#include "option/HelpOption.hpp"
#include "option/InfoOption.hpp"
#include "option/ListOption.hpp"
#include "option/RemoveOption.hpp"
#include "option/RunRendererOption.hpp"
#include "option/ShuffleOption.hpp"
#include "option/VersionOption.hpp"
#include "wallpaper_loader/PictureWallpaperLoader.hpp"
#include "wallpaper_loader/VideoWallpaperLoader.hpp"
#include "wallpaper_loader/WallpaperLoaderManager.hpp"

int main(const int argc, const char** argv) {
    DefaultWallpaper::createIfNotExists(true);
    PostSetScript::createIfNotExists();

    auto logger = std::make_shared<util::CliLogger>();
    auto config = std::make_shared<Config>(logger);
    config->load();

    auto wallpapers = std::make_shared<Wallpapers>();
    auto wallpaperLoader = std::make_shared<WallpaperLoaderManager>(wallpapers, config, logger);
    wallpaperLoader->addWallpaperLoader<PictureWallpaper>(std::make_unique<PictureWallpaperLoader>(logger));
    wallpaperLoader->addWallpaperLoader<VideoWallpaper>(std::make_unique<VideoWallpaperLoader>(logger));

    wallpaperLoader->loadWallpapers();
    wallpapers->sortByName();

    auto cliExecutor = std::make_shared<CliExecutor>(logger);
    cliExecutor->addOption("add", std::make_unique<AddOption>(wallpaperLoader, config, logger));
    cliExecutor->addOption("apply", std::make_unique<ApplyOption>(wallpapers, logger));
    cliExecutor->addOption("count", std::make_unique<CountOption>(wallpapers, logger));
    cliExecutor->addOption("help", std::make_unique<HelpOption>(cliExecutor->getOptions(), logger), {Flags::json});
    cliExecutor->addOption("info", std::make_unique<InfoOption>(wallpapers, logger), {Flags::json});
    cliExecutor->addOption("list", std::make_unique<ListOption>(wallpapers, logger), {Flags::json});
    cliExecutor->addOption("remove", std::make_unique<RemoveOption>(wallpapers, logger));
    cliExecutor->addOption("run-renderer", std::make_unique<RunRendererOption>(wallpapers, logger));
    cliExecutor->addOption("shuffle", std::make_unique<ShuffleOption>(wallpapers, logger));
    cliExecutor->addOption("version", std::make_unique<VersionOption>(logger), {Flags::json});
    cliExecutor->addHandler("apply", std::make_unique<ApplyHandler>(wallpapers));
    cliExecutor->addHandler("shuffle", std::make_unique<ShuffleHandler>(wallpapers));

    return cliExecutor->execute(argc, argv);
}
