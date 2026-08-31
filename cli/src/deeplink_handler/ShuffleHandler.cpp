// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/ShuffleHandler.hpp"

#include <ranges>

ShuffleHandler::ShuffleHandler(sptr<WallpaperRepository> wallpaperRepository) : wallpaperRepository(std::move(wallpaperRepository)) {}

int ShuffleHandler::handle(const boost::url_view& url) const {
    if(wallpaperRepository->count() < 1) {
        return 0;
    }

    if(!url.path().empty()) {
        return 1;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    if(auto includeParam = url.params().find("include"); includeParam != url.params().end()) {
        for(const auto& includeTag : (*includeParam).value | std::views::split(',')) {
            includeTags.emplace_back(includeTag.begin(), includeTag.end());
        }
    }

    if(auto excludeParam = url.params().find("exclude"); excludeParam != url.params().end()) {
        for(const auto& excludeTag : (*excludeParam)->value | std::views::split(',')) {
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
