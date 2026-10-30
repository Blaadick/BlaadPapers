// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "option/ShuffleOption.hpp"

#include "wallpaper/Wallpaper.hpp"

ShuffleOption::ShuffleOption(
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<util::Logger> logger
) : Option("Sets the random wallpaper"), wallpaperRepository(std::move(wallpaperRepository)), logger(std::move(logger)) {}

auto ShuffleOption::getUsageStrings() const noexcept -> std::vector<std::string_view> {
    return {
        "[include_tags] [exclude_tags]",
        R"('["General", "Nature"]' '["Explicit"]')"
    };
}

auto ShuffleOption::execute(
    const std::vector<std::string_view>& arguments,
    const std::unordered_set<sptr<Flag>>& flags
) -> int {
    if(wallpaperRepository->count() == 0) {
        logger->logInfo("No Wallpapers");
        return 0;
    }

    std::vector<std::string> includeTags;
    std::vector<std::string> excludeTags;

    if(!arguments.empty()) {
        auto doc = yyjson_read(arguments[0].data(), arguments[0].size(), YYJSON_READ_NOFLAG);
        if(!doc) {
            logger->logError("Failed to parse include tags");
            return 2;
        }

        auto root = yyjson_doc_get_root(doc);
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
        auto doc = yyjson_read(arguments[1].data(), arguments[1].size(), YYJSON_READ_NOFLAG);
        if(!doc) {
            logger->logError("Failed to parse exclude tags");
            return 2;
        }

        auto root = yyjson_doc_get_root(doc);
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

    const auto wallpaperToApply = wallpaperRepository->shuffle(
        includeTags,
        excludeTags
    );

    if(!wallpaperToApply) {
        logger->logWarning("No wallpapers found");
        return 1;
    }

    auto result = wallpaperRepository->apply(wallpaperToApply.value()->getId());
    if(result.has_value()) {
        logger->logInfo(std::format("Wallpaper \"{}\" applied", wallpaperToApply.value()->getId()));
        return 0;
    }

    logger->logError(std::format("Failed to apply wallpaper \"{}\": {}", wallpaperToApply.value()->getId(), result.error()));
    return 2;
}
