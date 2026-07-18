// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/ShuffleHandler.hpp"

ShuffleHandler::ShuffleHandler(sptr<Wallpapers> wallpapers) : wallpapers(std::move(wallpapers)) {}

int ShuffleHandler::handle(const Url& url) const {
    if(wallpapers->count() < 1) {
        return 0;
    }

    if(!url.path.empty()) {
        return 1;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    const auto includeIt = url.queries.find("include");
    if(includeIt != url.queries.end()) {
        for(const auto& includeTag : includeIt->second | std::views::split(',')) {
            includeTags.emplace_back(includeTag.begin(), includeTag.end());
        }
    }

    const auto excludeIt = url.queries.find("exclude");
    if(excludeIt != url.queries.end()) {
        for(const auto& includeTag : excludeIt->second | std::views::split(',')) {
            excludeTags.emplace_back(includeTag.begin(), includeTag.end());
        }
    }

    const auto wallpaperToApply = wallpapers->shuffle(
        includeTags.empty() ? std::nullopt : std::optional(includeTags),
        excludeTags.empty() ? std::nullopt : std::optional(excludeTags)
    );

    if(!wallpaperToApply) {
        return 1;
    }

    if(wallpapers->apply(wallpaperToApply->getId())) {
        return 0;
    }

    return 1;
}
