// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "deeplink_handler/ShuffleHandler.hpp"

ShuffleHandler::ShuffleHandler(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

int ShuffleHandler::handle(const Url& url) const {
    if(wallpapers->count() < 1) {
        logger->logInfo("No Wallpapers");
        return 0;
    }

    if(!url.path.empty() || !url.fragment.empty()) {
        logger->logWarning("Unexpected something other than shuffle queries");
        return 1;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    const auto includeIt = url.queries.find("include");
    if(includeIt != url.queries.end()) {
        const auto includeTagsData = nlohmann::json::parse(includeIt->second);
        if(includeTagsData.is_discarded()) {
            logger->logError("Failed to parse include tags");
            return 2;
        }

        includeTags = includeTagsData;
    }

    const auto excludeIt = url.queries.find("exclude");
    if(excludeIt != url.queries.end()) {
        std::vector<const Wallpaper*> filteredWallpapers;

        const auto excludeTagsData = nlohmann::json::parse(excludeIt->second);
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
