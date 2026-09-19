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

    auto addWallpaper(
        const std::filesystem::path& filePath,
        const std::filesystem::path& destinationFolderPath
    ) -> bool;

    void addWallpapers(
        const std::vector<std::filesystem::path>& paths,
        const std::filesystem::path& destinationFolderPath
    );

    auto getWallpaperLoaders() const -> const std::unordered_map<std::type_index, uptr<WallpaperLoader>>&;

    template<std::derived_from<Wallpaper> T>
    void addWallpaperLoader(uptr<WallpaperLoader> wallpaperLoader) {
        wallpaperLoaders.emplace(typeid(T), std::move(wallpaperLoader));
    }

    auto getSupportedFileTypes() const -> const std::unordered_set<const file::FileType*>&;

private:
    sptr<WallpaperRepository> wallpaperRepository;
    sptr<Config> config;
    sptr<util::Logger> logger;

    std::unordered_map<std::type_index, uptr<WallpaperLoader>> wallpaperLoaders;
};
