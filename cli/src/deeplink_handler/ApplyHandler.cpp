// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "deeplink_handler/ApplyHandler.hpp"

#include <ranges>

ApplyHandler::ApplyHandler(
    sptr<WallpaperRepository> wallpaperRepository
) : wallpaperRepository(std::move(wallpaperRepository)) {}

auto ApplyHandler::handle(const Uri& uri) const -> int {
    auto path = uri.path();
    if(!path.has_value()) {
        return 1;
    }

    auto wallpaperId = precentDecode(path->subview(1));
    if(wallpaperRepository->apply(wallpaperId)) {
        return 0;
    }

    return 1;
}
