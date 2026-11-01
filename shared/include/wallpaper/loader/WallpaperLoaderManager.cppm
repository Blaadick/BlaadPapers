// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:WallpaperLoaderManager;

import std;
import :Config;
import :Logger;
import :Pointers;
import :ShellExecutor;
import :Wallpaper;
import :WallpaperData;
import :WallpaperLoader;
import :WallpaperRepository;

export class WallpaperLoaderManager final {
public:
    WallpaperLoaderManager(
        sptr<WallpaperRepository> wallpaperRepository,
        sptr<Config> config,
        sptr<ShellExecutor> shellExecutor,
        sptr<util::Logger> logger
    );

    auto installWallpaper(
        const std::filesystem::path& filePath,
        std::optional<WallpaperData> wallpaperData = std::nullopt
    ) const -> std::expected<sptr<Wallpaper>, std::string>;

    auto loadWallpaper(std::filesystem::path wallpaperFilePath) const -> std::expected<sptr<Wallpaper>, std::string>;

    void loadWallpapers() const;

    auto getWallpaperLoaders() const -> const std::unordered_map<std::type_index, uptr<WallpaperLoader>>&;

    template<std::derived_from<Wallpaper> T>
    void addWallpaperLoader(uptr<WallpaperLoader> wallpaperLoader) {
        wallpaperLoaders.emplace(typeid(T), std::move(wallpaperLoader));
    }

private:
    sptr<WallpaperRepository> wallpaperRepository;
    sptr<Config> config;
    sptr<ShellExecutor> shellExecutor;
    sptr<util::Logger> logger;

    std::unordered_map<std::type_index, uptr<WallpaperLoader>> wallpaperLoaders;
};
