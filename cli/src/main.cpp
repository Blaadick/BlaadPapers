// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CliExecutor.hpp"
#include "DefaultWallpaper.hpp"
#include "ShellExecutor.hpp"
#include "WallpaperRepository.hpp"
#include "config/Config.hpp"
#include "deeplink_handler/ApplyHandler.hpp"
#include "deeplink_handler/ShuffleHandler.hpp"
#include "flag/Flags.hpp"
#include "logger/CliLogger.hpp"
#include "network/FetchManager.hpp"
#include "network/file/FileFetcher.hpp"
#include "network/http/HttpClient.hpp"
#include "network/http/HttpFetcher.hpp"
#include "option/ApplyOption.hpp"
#include "option/CountOption.hpp"
#include "option/HelpOption.hpp"
#include "option/InfoOption.hpp"
#include "option/InstallOption.hpp"
#include "option/ListOption.hpp"
#include "option/RemoveOption.hpp"
#include "option/RunRendererOption.hpp"
#include "option/ShuffleOption.hpp"
#include "option/VersionOption.hpp"
#include "wallpaper/PictureWallpaper.hpp"
#include "wallpaper/VideoWallpaper.hpp"
#include "wallpaper/loader/PictureWallpaperLoader.hpp"
#include "wallpaper/loader/VideoWallpaperLoader.hpp"
#include "wallpaper/loader/WallpaperLoaderManager.hpp"

auto main(int argc, char* argv[]) -> int {
    if(!std::filesystem::exists(DefaultWallpaper::defaultWallpaperFilePath())) {
        DefaultWallpaper::create();
    }

    auto logger = std::make_shared<util::CliLogger>();
    auto shellExecutor = std::make_shared<ShellExecutor>(logger);
    auto httpClient = std::make_shared<HttpClient>();
    auto fetchManager = std::make_shared<FetchManager>();
    fetchManager->addFetcher("http", std::make_unique<HttpFetcher>(httpClient, logger));
    fetchManager->addFetcher("https", std::make_unique<HttpFetcher>(httpClient, logger));
    fetchManager->addFetcher("file", std::make_unique<FileFetcher>());

    auto config = std::make_shared<Config>(logger);
    config->load();

    auto wallpaperRepository = std::make_shared<WallpaperRepository>(config, shellExecutor);
    auto wallpaperLoaderManager = std::make_shared<WallpaperLoaderManager>(wallpaperRepository, config, shellExecutor, logger);
    wallpaperLoaderManager->addWallpaperLoader<PictureWallpaper>(std::make_unique<PictureWallpaperLoader>(config));
    wallpaperLoaderManager->addWallpaperLoader<VideoWallpaper>(std::make_unique<VideoWallpaperLoader>(config));

    wallpaperLoaderManager->loadWallpapers();
    wallpaperRepository->sortByName();

    auto cliExecutor = std::make_shared<CliExecutor>(logger);
    cliExecutor->addOption("apply", std::make_unique<ApplyOption>(wallpaperRepository, logger));
    cliExecutor->addOption("count", std::make_unique<CountOption>(wallpaperRepository, logger));
    cliExecutor->addOption("help", std::make_unique<HelpOption>(cliExecutor->getOptions(), logger), {Flags::json});
    cliExecutor->addOption("info", std::make_unique<InfoOption>(wallpaperRepository, logger), {Flags::json});
    cliExecutor->addOption("install", std::make_unique<InstallOption>(wallpaperLoaderManager, fetchManager, config, logger));
    cliExecutor->addOption("list", std::make_unique<ListOption>(wallpaperRepository, logger), {Flags::json});
    cliExecutor->addOption("remove", std::make_unique<RemoveOption>(wallpaperRepository, logger));
    cliExecutor->addOption("run-renderer", std::make_unique<RunRendererOption>(wallpaperRepository, logger));
    cliExecutor->addOption("shuffle", std::make_unique<ShuffleOption>(wallpaperRepository, logger));
    cliExecutor->addOption("version", std::make_unique<VersionOption>(logger), {Flags::json});
    cliExecutor->addHandler("apply", std::make_unique<ApplyHandler>(wallpaperRepository));
    cliExecutor->addHandler("shuffle", std::make_unique<ShuffleHandler>(wallpaperRepository));

    return cliExecutor->execute(argc, argv);
}
