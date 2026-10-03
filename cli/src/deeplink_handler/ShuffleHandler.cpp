// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "deeplink_handler/ShuffleHandler.hpp"

#include <ranges>

ShuffleHandler::ShuffleHandler(sptr<WallpaperRepository> wallpaperRepository) : wallpaperRepository(std::move(wallpaperRepository)) {}

auto ShuffleHandler::handle(const Uri& uri) const -> int {
    if(wallpaperRepository->count() == 0) {
        return 0;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    auto queries = uri.queries();
    if(!queries.empty()) {
        if(auto includeParam = queries.find("include"); includeParam != queries.end()) {
            for(const auto& includeTag : includeParam->second | std::views::split(',')) {
                auto encodedTag = std::string_view(includeTag.begin(), includeTag.end());
                includeTags.emplace_back(precentDecode(encodedTag));
            }
        }

        if(auto excludeParam = queries.find("exclude"); excludeParam != queries.end()) {
            for(const auto& excludeTag : excludeParam->second | std::views::split(',')) {
                auto encodedTag = std::string_view(excludeTag.begin(), excludeTag.end());
                excludeTags.emplace_back(precentDecode(encodedTag));
            }
        }
    }

    auto wallpaperToApply = wallpaperRepository->shuffle(
        includeTags.empty() ? std::vector<std::string>{} : includeTags,
        excludeTags.empty() ? std::vector<std::string>{} : excludeTags
    );

    if(!wallpaperToApply) {
        return 1;
    }

    if(wallpaperRepository->apply(wallpaperToApply.value()->getId())) {
        return 0;
    }

    return 1;
}
