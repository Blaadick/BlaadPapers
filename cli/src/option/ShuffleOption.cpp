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

std::vector<std::string_view> ShuffleOption::getUsageStrings() const {
    return {
        "[include_tags] [exclude_tags]",
        R"('["General", "Nature"]' '["Explicit"]')"
    };
}

int ShuffleOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) {
    if(wallpapers->count() == 0) {
        logger->logInfo("No Wallpapers");
        return 0;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    if(!arguments.empty()) {
        const auto doc = yyjson_read(arguments[0].data(), arguments.size(), YYJSON_READ_NOFLAG);
        if(!doc) {
            logger->logError("Failed to parse include tags");
            return 2;
        }

        const auto root = yyjson_doc_get_root(doc);
        if(!yyjson_is_arr(root)) {
            logger->logError("Failed to parse include tags");
            yyjson_doc_free(doc);
            return 2;
        }

        size_t i, max;
        yyjson_val* item;
        yyjson_arr_foreach(root, i, max, item) {
            if(yyjson_is_str(item)) {
                includeTags.emplace_back(yyjson_get_str(item));
            }
        }
    }

    if(arguments.size() >= 2) {
        const auto doc = yyjson_read(arguments[1].data(), arguments.size(), YYJSON_READ_NOFLAG);
        if(!doc) {
            logger->logError("Failed to parse exclude tags");
            return 2;
        }

        const auto root = yyjson_doc_get_root(doc);
        if(!yyjson_is_arr(root)) {
            logger->logError("Failed to parse exclude tags");
            yyjson_doc_free(doc);
            return 2;
        }

        size_t i, max;
        yyjson_val* item;
        yyjson_arr_foreach(root, i, max, item) {
            if(yyjson_is_str(item)) {
                excludeTags.emplace_back(yyjson_get_str(item));
            }
        }
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
