// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include "WallpaperRepository.hpp"
#include "config/Config.hpp"
#include "util/Pointers.hpp"
#include "wallpaper_loader/WallpaperLoader.hpp"

class WallpaperLoaderManager {
public:
    WallpaperLoaderManager(sptr<WallpaperRepository> wallpaperRepository, sptr<Config> config, sptr<util::Logger> logger);

    void loadWallpapers();

    bool addWallpaper(
        const std::filesystem::path& filePath,
        const std::filesystem::path& destinationFolderPath
    );

    void addWallpapers(
        const std::vector<std::filesystem::path>& paths,
        const std::filesystem::path& destinationFolderPath
    );

    const std::unordered_map<std::type_index, uptr<WallpaperLoader>>& getWallpaperLoaders() const;

    template<std::derived_from<Wallpaper> T>
    void addWallpaperLoader(uptr<WallpaperLoader> wallpaperLoader) {
        wallpaperLoaders.emplace(typeid(T), std::move(wallpaperLoader));
    }

    const std::unordered_set<const file::FileType*>& getSupportedFileTypes() const {
        static std::unordered_set<const file::FileType*> supportedFileTypes;

        if(supportedFileTypes.empty()) {
            for(const auto& wallpaperLoader : wallpaperLoaders | std::views::values) {
                auto loaderFileTypes = wallpaperLoader->getSupportedFileTypes();
                supportedFileTypes.insert(loaderFileTypes.begin(), loaderFileTypes.end());
            }
        }

        return supportedFileTypes;
    }

private:
    sptr<WallpaperRepository> wallpaperRepository;
    sptr<Config> config;
    sptr<util::Logger> logger;

    std::unordered_map<std::type_index, uptr<WallpaperLoader>> wallpaperLoaders;
};
