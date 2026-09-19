// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "deeplink_handler/ShuffleHandler.hpp"

#include <ranges>

ShuffleHandler::ShuffleHandler(sptr<WallpaperRepository> wallpaperRepository) : wallpaperRepository(std::move(wallpaperRepository)) {}

auto ShuffleHandler::handle(const Uri& uri) const -> int {
    if(wallpaperRepository->count() < 1) {
        return 0;
    }

    if(!uri.path().empty()) {
        return 1;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    if(auto includeParam = uri.queries().find("include"); includeParam != uri.queries().end()) {
        for(const auto& includeTag : includeParam->second | std::views::split(',')) {
            includeTags.emplace_back(includeTag.begin(), includeTag.end());
        }
    }

    if(auto excludeParam = uri.queries().find("exclude"); excludeParam != uri.queries().end()) {
        for(const auto& excludeTag : excludeParam->second | std::views::split(',')) {
            excludeTags.emplace_back(excludeTag.begin(), excludeTag.end());
        }
    }

    const auto wallpaperToApply = wallpaperRepository->shuffle(
        includeTags.empty() ? std::nullopt : std::optional(includeTags),
        excludeTags.empty() ? std::nullopt : std::optional(excludeTags)
    );

    if(!wallpaperToApply) {
        return 1;
    }

    if(wallpaperRepository->apply(wallpaperToApply->getId())) {
        return 0;
    }

    return 1;
}
