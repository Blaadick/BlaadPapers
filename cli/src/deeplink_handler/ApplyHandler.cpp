// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/ApplyHandler.hpp"

ApplyHandler::ApplyHandler(
    sptr<WallpaperRepository> wallpaperRepository
) : wallpaperRepository(std::move(wallpaperRepository)) {}

int ApplyHandler::handle(const Uri& uri) const {
    if(uri.path().empty()) {
        return 1;
    }

    if(uri.path().size() > 1) {
        return 1;
    }

    if(wallpaperRepository->apply(uri.path().back())) {
        return 0;
    }

    return 1;
}
