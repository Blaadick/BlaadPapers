// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/ApplyHandler.hpp"

ApplyHandler::ApplyHandler(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

int ApplyHandler::handle(const Url& url) const {
    if(url.path.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    if(url.path.size() > 1 || !url.queries.empty() || !url.fragment.empty()) {
        logger->logWarning("Unexpected something after wallpaper id");
        return 1;
    }

    if(wallpapers->apply(url.path[0])) {
        logger->logInfo("Wallpaper \"" + url.path[0] + "\" applied");
        return 0;
    }

    logger->logError("Wallpaper \"" + url.path[0] + "\" not found");
    return 2;
}
