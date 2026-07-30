// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <typeindex>
#include <unordered_map>
#include "Config.hpp"
#include "Wallpapers.hpp"
#include "util/Pointers.hpp"
#include "wallpaper_loader/WallpaperLoader.hpp"

class WallpaperLoaderManager {
public:
    WallpaperLoaderManager(sptr<Wallpapers> wallpapers, sptr<Config> config, sptr<util::Logger> logger);

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

private:
    std::unordered_map<std::type_index, uptr<WallpaperLoader>> wallpaperLoaders;

    sptr<Wallpapers> wallpapers;
    sptr<Config> config;
    sptr<util::Logger> logger;
};
