// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ShuffleOption.hpp"

#include <random>
#include "data/Wallpaper.hpp"

namespace rng = std::ranges;

ShuffleOption::ShuffleOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option(), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::string ShuffleOption::getHelpMessage() const {
    return "shuffle help";
}

int ShuffleOption::execute(const std::vector<std::string>& arguments) {
    if(wallpapers->count() < 1) {
        logger->logInfo("No Wallpapers");
        return 0;
    }

    std::mt19937 rng(std::random_device{}());
    const Wallpaper* wallpaperToApply;

    if(arguments.size() >= 1) {
        std::vector<std::string> includeTags;
        std::vector<std::string> excludeTags;
        std::vector<const Wallpaper*> filteredWallpapers;

        if(arguments.size() >= 1) {
            const auto excludeTagsData = nlohmann::json::parse(arguments[0]);
            if(excludeTagsData.is_discarded()) {
                logger->logError("Failed to parse exclude tags");
                return 2;
            }

            includeTags = excludeTagsData;
        }

        if(arguments.size() >= 2) {
            const auto includeTagsData = nlohmann::json::parse(arguments[1]);
            if(includeTagsData.is_discarded()) {
                logger->logError("Failed to parse include tags");
                return 2;
            }

            includeTags = includeTagsData;
        }

        for(const auto& wallpaper : *wallpapers) {
            const bool containsIncludeTags = rng::all_of(
                includeTags,
                [&wallpaper](const std::string& tag) {
                    return rng::contains(wallpaper->getTags(), tag);
                }
            );

            const bool containsExcludeTags = rng::any_of(
                excludeTags,
                [&wallpaper](const std::string& tag) {
                    return rng::contains(wallpaper->getTags(), tag);
                }
            );

            if(containsIncludeTags && !containsExcludeTags) {
                filteredWallpapers.emplace_back(wallpaper.get());
            }
        }

        if(filteredWallpapers.empty()) {
            logger->logInfo("No wallpapers found");
            return 0;
        }

        const auto randomIndex = std::uniform_int_distribution(0, static_cast<int>(filteredWallpapers.size()))(rng);
        wallpaperToApply = filteredWallpapers[randomIndex];
    } else {
        const auto randomIndex = std::uniform_int_distribution(0, wallpapers->count())(rng);
        wallpaperToApply = wallpapers->get(randomIndex);
    }

    if(wallpapers->apply(wallpaperToApply->getId())) {
        logger->logInfo("Wallpaper \"" + wallpaperToApply->getId() + "\" applied");
        return 0;
    }

    logger->logError("Wallpaper \"" + wallpaperToApply->getId() + "\" not found");
    return 2;
}
