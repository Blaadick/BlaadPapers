// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "wallpaper_loader/WallpaperLoaderManager.hpp"

#include <format>
#include <fstream>
#include <ranges>
#include "Config.hpp"
#include "WallpaperRepository.hpp"
#include "util/ImageUtils.hpp"
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

WallpaperLoaderManager::WallpaperLoaderManager(
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<Config> config,
    sptr<util::Logger> logger
) : wallpaperRepository(std::move(wallpaperRepository)), config(std::move(config)), logger(std::move(logger)) {}

void WallpaperLoaderManager::loadWallpapers() {
    wallpaperRepository->clear();

    if(!util::createDirIfNotExists(config->getWallpapersDirPath())) {
        logger->logError("Failed to create directory \"" + config->getWallpapersDirPath().string() + "\"");
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

            for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
                if(wallpaperLoader->isSupported(wallpaperDirEntry)) {
                    auto loadedWallpaper = wallpaperLoader->loadWallpaper(wallpaperDirEntry);

                    if(!loadedWallpaper) {
                        logger->logWarning(std::format("Failed to load wallpaper \"{}\"", wallpaperDirEntry.path()));
                        break;
                    }

                    wallpaperRepository->add(std::move(loadedWallpaper));
                }
            }
        }
    }
}

// TODO Refactor
bool WallpaperLoaderManager::addWallpaper(const fs::path& filePath, const fs::path& destinationFolderPath) {
    for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
        if(!wallpaperLoader->isSupported(filePath)) {
            continue;
        }

        const auto wallpaperId = filePath.stem().string();
        const auto wallpaperDirPath = destinationFolderPath / wallpaperId;

        if(fs::exists(wallpaperDirPath)) {
            logger->logWarning(
                std::format(
                    R"(Failed to add "{}" wallpaper: Wallpaper with same id already exists)",
                    wallpaperId
                )
            );

            return false;
        }

        if(!fs::create_directory(wallpaperDirPath)) {
            logger->logWarning(
                std::format(
                    R"(Failed to add "{}" wallpaper: Failed to create directory "{}")",
                    wallpaperId,
                    wallpaperDirPath.string()
                )
            );

            return false;
        }

        const auto wallpaperFilePath = wallpaperDirPath / ("wallpaper" + filePath.extension().string());
        if(!fs::copy_file(filePath, wallpaperFilePath)) {
            logger->logWarning(
                std::format(
                    R"(Failed to add "{}" wallpaper: Failed to copy wallpaper to "{}")",
                    wallpaperId,
                    wallpaperFilePath.string()
                )
            );

            fs::remove_all(wallpaperDirPath);
            return false;
        }

        return true;
    }

    return false;
}

void WallpaperLoaderManager::addWallpapers(const std::vector<fs::path>& paths, const fs::path& destinationFolderPath) {
    for(const auto& path : paths) {
        if(fs::is_directory(path)) {
            for(const auto& dirEntry : fs::directory_iterator(path)) {
                if(!dirEntry.is_regular_file()) {
                    continue;
                }

                addWallpaper(dirEntry.path(), destinationFolderPath);
            }

            continue;
        }

        if(fs::is_regular_file(path)) {
            addWallpaper(path, destinationFolderPath);
        }
    }
}

const std::unordered_map<std::type_index, uptr<WallpaperLoader>>& WallpaperLoaderManager::getWallpaperLoaders() const {
    return wallpaperLoaders;
}
