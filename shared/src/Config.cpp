// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Config.hpp"

#include <algorithm>
#include <fstream>
#include <yyjson.h>

Config::Config(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void Config::load() {
    const auto configFilePath = util::configFilePath();
    if(std::filesystem::exists(configFilePath)) {
        yyjson_read_err readErr;
        const auto doc = yyjson_read_file(configFilePath.c_str(), YYJSON_READ_NOFLAG, nullptr, &readErr);
        if(!doc) {
            logger->logError("Failed to parse config file. Fallback to defaults");
            return;
        }

        const auto root = yyjson_doc_get_root(doc);
        if(!yyjson_is_obj(root)) {
            logger->logError("Failed to parse config file. Fallback to defaults");
            yyjson_doc_free(doc);
            return;
        }

        const auto badTagsData = yyjson_obj_get(root, "bad_tags");
        if(yyjson_is_arr(badTagsData)) {
            badTags.clear();

            size_t i, max;
            yyjson_val* item;
            yyjson_arr_foreach(badTagsData, i, max, item) {
                if(yyjson_is_str(item)) {
                    badTags.emplace_back(yyjson_get_str(item));
                }
            }
        }

        const auto wallpapersDirData = yyjson_obj_get(root, "wallpapers_path");
        if(yyjson_is_str(wallpapersDirData)) {
            wallpapersDirPath = unsafe_yyjson_get_str(wallpapersDirData);
        }

        const auto statusBarVisibleData = yyjson_obj_get(root, "status_bar_visible");
        if(yyjson_is_bool(statusBarVisibleData)) {
            isStatusBarVisible = unsafe_yyjson_get_bool(statusBarVisibleData);
        }

        yyjson_doc_free(doc);
    } else {
        saveConfig();
    }
}

std::vector<std::string> Config::getBadTags() const {
    return badTags;
}

std::filesystem::path Config::getWallpapersDirPath() const {
    return wallpapersDirPath;
}

bool Config::getStatusBarVisible() const {
    return isStatusBarVisible;
}

void Config::setStatusBarVisible(const bool newVisibility) {
    isStatusBarVisible = newVisibility;
    saveConfig();
}

bool Config::isWallpaperBad(const Wallpaper& wallpaper) const {
    return std::ranges::any_of(
        badTags,
        [&wallpaper](const std::string& tag) {
            return std::ranges::contains(wallpaper.getTags(), tag);
        }
    );
}

void Config::saveConfig() const {
    const auto doc = yyjson_mut_doc_new(nullptr);
    const auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    const auto badTagsData = yyjson_mut_arr(doc);
    for(const auto& tag : badTags) {
        yyjson_mut_arr_add_str(doc, badTagsData, tag.c_str());
    }

    yyjson_mut_obj_add_val(doc, root, "bad_tags", badTagsData);
    yyjson_mut_obj_add_str(doc, root, "wallpapers_path", wallpapersDirPath.c_str());
    yyjson_mut_obj_add_bool(doc, root, "status_bar_visible", isStatusBarVisible);

    yyjson_write_err writeErr;
    const auto isWritten = yyjson_mut_write_file(util::configFilePath().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError("Failed to write config file to \"" + util::configFilePath().string() + '\"');
    }

    yyjson_mut_doc_free(doc);
}
