// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "CliExecutor.hpp"
#include "DefaultWallpaper.hpp"
#include "PostSetScript.hpp"
#include "WallpaperRepository.hpp"
#include "config/Config.hpp"
#include "data/PictureWallpaper.hpp"
#include "data/VideoWallpaper.hpp"
#include "deeplink_handler/ApplyHandler.hpp"
#include "deeplink_handler/ShuffleHandler.hpp"
#include "deeplink_handler/WallhavenHandler.hpp"
#include "flag/Flags.hpp"
#include "logger/CliLogger.hpp"
#include "network/HttpClient.hpp"
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

    auto wallpaperRepository = std::make_shared<WallpaperRepository>();
    auto wallpaperLoader = std::make_shared<WallpaperLoaderManager>(wallpaperRepository, config, logger);
    wallpaperLoader->addWallpaperLoader<PictureWallpaper>(std::make_unique<PictureWallpaperLoader>(logger));
    wallpaperLoader->addWallpaperLoader<VideoWallpaper>(std::make_unique<VideoWallpaperLoader>(logger));

    wallpaperLoader->loadWallpapers();
    wallpaperRepository->sortByName();

    auto httpClient = std::make_shared<HttpClient>();

    auto cliExecutor = std::make_shared<CliExecutor>(logger);
    cliExecutor->addOption("add", std::make_unique<AddOption>(wallpaperLoader, httpClient, config, logger));
    cliExecutor->addOption("apply", std::make_unique<ApplyOption>(wallpaperRepository, logger));
    cliExecutor->addOption("count", std::make_unique<CountOption>(wallpaperRepository, logger));
    cliExecutor->addOption("help", std::make_unique<HelpOption>(cliExecutor->getOptions(), wallpaperLoader, logger), {Flags::json});
    cliExecutor->addOption("info", std::make_unique<InfoOption>(wallpaperRepository, logger), {Flags::json});
    cliExecutor->addOption("list", std::make_unique<ListOption>(wallpaperRepository, logger), {Flags::json});
    cliExecutor->addOption("remove", std::make_unique<RemoveOption>(wallpaperRepository, logger));
    cliExecutor->addOption("run-renderer", std::make_unique<RunRendererOption>(wallpaperRepository, logger));
    cliExecutor->addOption("shuffle", std::make_unique<ShuffleOption>(wallpaperRepository, logger));
    cliExecutor->addOption("version", std::make_unique<VersionOption>(logger), {Flags::json});
    cliExecutor->addHandler("apply", std::make_unique<ApplyHandler>(wallpaperRepository));
    cliExecutor->addHandler("shuffle", std::make_unique<ShuffleHandler>(wallpaperRepository));
    cliExecutor->addHandler("wallhaven.cc", std::make_unique<WallhavenHandler>(wallpaperLoader, config, httpClient));

    return cliExecutor->execute(argc, argv);
}
