// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper/loader/WallpaperLoaderManager.hpp"

#include <format>
#include <ranges>
#include "WallpaperRepository.hpp"
#include "config/Config.hpp"
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

WallpaperLoaderManager::WallpaperLoaderManager(
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<Config> config,
    sptr<ShellExecutor> shellExecutor,
    sptr<util::Logger> logger
) : wallpaperRepository(std::move(wallpaperRepository)), config(std::move(config)), shellExecutor(std::move(shellExecutor)), logger(std::move(logger)) {}

auto WallpaperLoaderManager::installWallpaper(
    const fs::path& filePath,
    std::optional<WallpaperData> wallpaperData
) const -> std::expected<sptr<Wallpaper>, std::string> {
    for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
        if(!wallpaperLoader->isSupported(filePath)) {
            continue;
        }

        auto wallpaper = wallpaperLoader->installWallpaper(filePath, std::move(wallpaperData));

        if(wallpaper.has_value()) {
            shellExecutor->executeWithWallpaperData(config->getPostInstallScript(), **wallpaper);
        }

        return wallpaper;
    }

    return std::unexpected("No supported wallpaper loader found");
}

auto WallpaperLoaderManager::loadWallpaper(std::filesystem::path wallpaperFilePath) const -> std::expected<sptr<Wallpaper>, std::string> {
    for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
        if(!wallpaperLoader->isSupported(wallpaperFilePath)) {
            continue;
        }

        return wallpaperLoader->loadWallpaper(std::move(wallpaperFilePath));
    }

    return std::unexpected("No supported wallpaper loader found");
}

void WallpaperLoaderManager::loadWallpapers() const {
    wallpaperRepository->clear();

    if(!util::createDirIfNotExists(config->getWallpapersDirPath())) {
        logger->logError(std::format("Failed to create directory \"{}\"", config->getWallpapersDirPath()));
        return;
    }

    for(const auto& wallpapersDirEntry : fs::directory_iterator(config->getWallpapersDirPath())) {
        if(!wallpapersDirEntry.is_directory()) {
            continue;
        }

        for(const auto& wallpaperDirEntry : fs::directory_iterator(wallpapersDirEntry.path())) {
            if(wallpaperDirEntry.path().stem() != "wallpaper") {
                continue;
            }

            auto loadedWallpaper = loadWallpaper(wallpaperDirEntry);
            if(!loadedWallpaper) {
                logger->logWarning(std::format("Failed to load wallpaper \"{}\": {}", wallpaperDirEntry.path(), loadedWallpaper.error()));
                break;
            }

            wallpaperRepository->add(std::move(*loadedWallpaper));
            break;
        }
    }
}

auto WallpaperLoaderManager::getWallpaperLoaders() const -> const std::unordered_map<std::type_index, uptr<WallpaperLoader>>& {
    return wallpaperLoaders;
}
