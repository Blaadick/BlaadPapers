// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "data/Size.hpp"
#include "deeplink_handler/DeeplinkHandler.hpp"
#include "network/HttpClient.hpp"
#include "wallpaper_loader/WallpaperLoaderManager.hpp"

struct WallhavenFileData {
    std::string url;
    std::string id;
    std::string source;
    Size resolution;
};

class WallhavenHandler final : public DeeplinkHandler {
public:
    WallhavenHandler(sptr<WallpaperLoaderManager> wallpaperLoader, sptr<Config> config, sptr<HttpClient> httpClient);

    std::optional<WallhavenFileData> getWallpaperData(const Url& url) const;

    int handle(const Url& url) const override;

private:
    sptr<WallpaperLoaderManager> wallpaperLoader;
    sptr<Config> config;
    sptr<HttpClient> httpClient;
};
