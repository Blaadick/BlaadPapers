// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include "WallpaperRepository.hpp"
#include "config/Config.hpp"
#include "util/Pointers.hpp"
#include "wallpaper_loader/WallpaperLoader.hpp"

class WallpaperLoaderManager final {
public:
    WallpaperLoaderManager(
        sptr<WallpaperRepository> wallpaperRepository,
        sptr<Config> config,
        sptr<util::Logger> logger
    );

    void loadWallpapers() const;

    auto installWallpaper(
        const std::filesystem::path& filePath,
        std::optional<WallpaperData> wallpaperData = std::nullopt
    ) const -> std::expected<uptr<Wallpaper>, std::string>;

    auto loadWallpaper(const std::filesystem::path& wallpaperFilePath) const -> std::expected<uptr<Wallpaper>, std::string>;

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
