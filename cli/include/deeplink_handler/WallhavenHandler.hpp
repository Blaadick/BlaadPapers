// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "deeplink_handler/DeeplinkHandler.hpp"
#include "network/HttpWorker.hpp"
#include "wallpaper_loader/WallpaperLoaderManager.hpp"

class WallhavenHandler final : public DeeplinkHandler {
public:
    WallhavenHandler(sptr<WallpaperLoaderManager> wallpaperLoader, sptr<HttpWorker> httpWorker);

    int handle(const Url& url) const override;

private:
    sptr<WallpaperLoaderManager> wallpaperLoader;
    sptr<HttpWorker> httpWorker;
};
