// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper_loader/WallpaperLoaderManager.hpp"

#include <format>
#include <fstream>
#include <ranges>
#include "config/Config.hpp"
#include "WallpaperRepository.hpp"
#include "util/ImageUtils.hpp"
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

WallpaperLoaderManager::WallpaperLoaderManager(
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<Config> config,
    sptr<util::Logger> logger
) : wallpaperRepository(std::move(wallpaperRepository)), config(std::move(config)), logger(std::move(logger)) {}

auto WallpaperLoaderManager::installWallpaper(
    const fs::path& filePath,
    std::optional<WallpaperData> wallpaperData
) const -> std::expected<uptr<Wallpaper>, std::string> {
    for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
        if(!wallpaperLoader->isSupported(filePath)) {
            continue;
        }

        return wallpaperLoader->installWallpaper(filePath, config->getWallpapersDirPath(), std::move(wallpaperData));
    }

    return std::unexpected("No supported wallpaper loader found");
}

auto WallpaperLoaderManager::loadWallpaper(const std::filesystem::path& wallpaperFilePath) const -> std::expected<uptr<Wallpaper>, std::string> {
    for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
        if(!wallpaperLoader->isSupported(wallpaperFilePath)) {
            continue;
        }

        return wallpaperLoader->loadWallpaper(wallpaperFilePath);
    }

    return std::unexpected("No supported wallpaper loader found");
}

void WallpaperLoaderManager::loadWallpapers() const {
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

            auto loadedWallpaper = loadWallpaper(wallpaperDirEntry);

            if(!loadedWallpaper) {
                logger->logWarning(std::format("Failed to load wallpaper \"{}\"", wallpaperDirEntry.path()));
                break;
            }

            wallpaperRepository->add(std::move(*loadedWallpaper));
        }
    }
}

auto WallpaperLoaderManager::getWallpaperLoaders() const -> const std::unordered_map<std::type_index, uptr<WallpaperLoader>>& {
    return wallpaperLoaders;
}

auto WallpaperLoaderManager::getSupportedFileTypes() const -> const std::unordered_set<const file::FileType*>& {
    static std::unordered_set<const file::FileType*> supportedFileTypes;

    if(supportedFileTypes.empty()) {
        for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
            auto loaderFileTypes = wallpaperLoader->getSupportedFileTypes();
            supportedFileTypes.insert(loaderFileTypes.begin(), loaderFileTypes.end());
        }
    }

    return supportedFileTypes;
}
