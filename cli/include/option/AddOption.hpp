// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "logger/Logger.hpp"
#include "network/DownloadManager.hpp"
#include "option/Option.hpp"
#include "wallpaper_loader/WallpaperLoaderManager.hpp"

class AddOption final : public Option {
public:
    AddOption(
        sptr<WallpaperLoaderManager> wallpaperLoader,
        sptr<DownloadManager> downloadManager,
        sptr<Config> config,
        sptr<util::Logger> logger
    );

    auto getUsageStrings() const noexcept -> std::vector<std::string_view> override;

    auto execute(
        const std::vector<std::string_view>& arguments,
        const std::unordered_set<sptr<Flag>>& flags
    ) -> int override;

private:
    sptr<WallpaperLoaderManager> wallpaperLoader;
    sptr<DownloadManager> downloadManager;
    sptr<Config> config;
    sptr<util::Logger> logger;
};
