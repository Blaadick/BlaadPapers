// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/ApplyHandler.hpp"

ApplyHandler::ApplyHandler(sptr<Wallpapers> wallpapers) : wallpapers(std::move(wallpapers)) {}

int ApplyHandler::handle(const Url& url) const {
    if(url.path.empty()) {
        return 1;
    }

    if(url.path.size() > 1) {
        return 1;
    }

    if(wallpapers->apply(url.path[0])) {
        return 0;
    }

    return 1;
}
