// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ShuffleOption.hpp"

#include <random>
#include "data/Wallpaper.hpp"

namespace rng = std::ranges;

ShuffleOption::ShuffleOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option("Sets the random wallpaper"), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

int ShuffleOption::execute(const std::vector<std::string_view>& arguments) {
    if(wallpapers->count() == 0) {
        logger->logInfo("No Wallpapers");
        return 0;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    if(!arguments.empty()) {
        const auto includeTagsData = nlohmann::json::parse(arguments[0]);
        if(includeTagsData.is_discarded()) {
            logger->logError("Failed to parse include tags");
            return 2;
        }

        includeTags = includeTagsData;
    }

    if(arguments.size() >= 2) {
        const auto excludeTagsData = nlohmann::json::parse(arguments[1]);
        if(excludeTagsData.is_discarded()) {
            logger->logError("Failed to parse exclude tags");
            return 2;
        }

        excludeTags = excludeTagsData;
    }

    const auto wallpaperToApply = wallpapers->shuffle(
        includeTags.empty() ? std::nullopt : std::optional(includeTags),
        excludeTags.empty() ? std::nullopt : std::optional(excludeTags)
    );

    if(!wallpaperToApply) {
        logger->logWarning("No wallpapers found");
        return 1;
    }

    if(wallpapers->apply(wallpaperToApply->getId())) {
        logger->logInfo("Wallpaper \"" + wallpaperToApply->getId() + "\" applied");
        return 0;
    }

    logger->logError("Wallpaper \"" + wallpaperToApply->getId() + "\" not found");
    return 2;
}
