// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "config/Config.hpp"

#include <algorithm>
#include <fstream>
#include <yyjson.h>

Config::Config(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void Config::load() {
    loadGeneral();
    loadGui();
    loadApi();
}

void Config::loadGeneral() {
    if(!std::filesystem::exists(generalConfigFilePath())) {
        saveGeneral();
        return;
    }

    yyjson_read_err readErr;
    auto doc = yyjson_read_file(generalConfigFilePath().c_str(), YYJSON_READ_NOFLAG, nullptr, &readErr);
    if(!doc) {
        logger->logError("Failed to parse config file. Fallback to defaults");
        return;
    }

    auto root = yyjson_doc_get_root(doc);
    if(yyjson_is_obj(root)) {
        auto wallpapersDirData = yyjson_obj_get(root, "wallpapers_path");
        if(yyjson_is_str(wallpapersDirData)) {
            wallpapersDirPath = unsafe_yyjson_get_str(wallpapersDirData);
        }

        auto badTagsData = yyjson_obj_get(root, "bad_tags");
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
    } else {
        logger->logError("Failed to parse config file. Fallback to defaults");
    }

    yyjson_doc_free(doc);
}

void Config::loadGui() {
    if(!std::filesystem::exists(guiConfigFilePath())) {
        saveGui();
        return;
    }

    yyjson_read_err readErr;
    auto doc = yyjson_read_file(guiConfigFilePath().c_str(), YYJSON_READ_NOFLAG, nullptr, &readErr);
    if(!doc) {
        logger->logError("Failed to parse config file. Fallback to defaults");
        return;
    }

    auto root = yyjson_doc_get_root(doc);
    if(yyjson_is_obj(root)) {
        auto statusBarVisibleData = yyjson_obj_get(root, "status_bar_visible");
        if(yyjson_is_bool(statusBarVisibleData)) {
            isStatusBarVisible = unsafe_yyjson_get_bool(statusBarVisibleData);
        }
    } else {
        logger->logError("Failed to parse config file. Fallback to defaults");
    }

    yyjson_doc_free(doc);
}

void Config::loadApi() {
    if(!std::filesystem::exists(apiConfigFilePath())) {
        saveApi();
        return;
    }

    yyjson_read_err readErr;
    auto doc = yyjson_read_file(apiConfigFilePath().c_str(), YYJSON_READ_NOFLAG, nullptr, &readErr);
    if(!doc) {
        logger->logError("Failed to parse config file. Fallback to defaults");
        return;
    }

    auto root = yyjson_doc_get_root(doc);
    if(yyjson_is_obj(root)) {
        auto wallhavenApiKeyData = yyjson_obj_get(root, "wallhaven");
        if(yyjson_is_str(wallhavenApiKeyData)) {
            wallhavenApiKey = yyjson_get_str(wallhavenApiKeyData);
        }

        auto danbooruData = yyjson_obj_get(root, "danbooru");
        if(yyjson_is_obj(danbooruData)) {
            auto danbooruLoginData = yyjson_obj_get(danbooruData, "login");
            if(yyjson_is_str(danbooruLoginData)) {
                danbooruLogin = yyjson_get_str(danbooruLoginData);
            }

            auto danbooruApiKeyData = yyjson_obj_get(danbooruData, "key");
            if(yyjson_is_str(danbooruApiKeyData)) {
                danbooruApiKey = yyjson_get_str(danbooruApiKeyData);
            }
        }
    } else {
        logger->logError("Failed to parse config file. Fallback to defaults");
    }

    yyjson_doc_free(doc);
}

void Config::save() const {
    saveGeneral();
    saveGui();
    saveApi();
}

void Config::saveGeneral() const {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    auto badTagsData = yyjson_mut_arr(doc);
    for(const auto& tag : badTags) {
        yyjson_mut_arr_add_str(doc, badTagsData, tag.c_str());
    }

    yyjson_mut_obj_add_str(doc, root, "wallpapers_path", wallpapersDirPath.c_str());
    yyjson_mut_obj_add_val(doc, root, "bad_tags", badTagsData);

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(generalConfigFilePath().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError("Failed to write config file to \"" + generalConfigFilePath().string() + '\"');
    }

    yyjson_mut_doc_free(doc);
}

void Config::saveGui() const {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    yyjson_mut_obj_add_bool(doc, root, "status_bar_visible", isStatusBarVisible);

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(guiConfigFilePath().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError("Failed to write config file to \"" + generalConfigFilePath().string() + '\"');
    }

    yyjson_mut_doc_free(doc);
}

void Config::saveApi() const {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    if(wallhavenApiKey.has_value()) {
        yyjson_mut_obj_add_str(doc, root, "wallhaven", wallhavenApiKey->c_str());
    } else {
        yyjson_mut_obj_add_null(doc, root, "wallhaven");
    }

    auto danbooruData = yyjson_mut_obj_add_obj(doc, root, "danbooru");
    if(danbooruLogin.has_value()) {
        yyjson_mut_obj_add_str(doc, danbooruData, "login", danbooruLogin->c_str());
    } else {
        yyjson_mut_obj_add_null(doc, danbooruData, "login");
    }
    if(danbooruApiKey.has_value()) {
        yyjson_mut_obj_add_str(doc, danbooruData, "key", danbooruApiKey->c_str());
    } else {
        yyjson_mut_obj_add_null(doc, danbooruData, "key");
    }

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(apiConfigFilePath().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError("Failed to write config file to \"" + generalConfigFilePath().string() + '\"');
    }

    yyjson_mut_doc_free(doc);
}

const std::filesystem::path& Config::getWallpapersDirPath() const {
    return wallpapersDirPath;
}

const std::vector<std::string>& Config::getBadTags() const {
    return badTags;
}

bool Config::isWallpaperBad(const Wallpaper& wallpaper) const {
    return std::ranges::any_of(
        badTags,
        [&wallpaper](const std::string& tag) {
            return std::ranges::contains(wallpaper.getTags(), tag);
        }
    );
}

const std::optional<std::string>& Config::getWallhavenApiKey() const {
    return wallhavenApiKey;
}

const std::optional<std::string>& Config::getDanbooruLogin() const {
    return danbooruLogin;
}

const std::optional<std::string>& Config::getDanbooruApiKey() const {
    return danbooruApiKey;
}

bool Config::getStatusBarVisible() const {
    return isStatusBarVisible;
}

void Config::setStatusBarVisible(const bool newVisibility) {
    isStatusBarVisible = newVisibility;
    saveGui();
}

const std::filesystem::path& Config::generalConfigFilePath() const {
    static const auto generalConfigFilePath = util::configDir() / "config.json";
    return generalConfigFilePath;
}

const std::filesystem::path& Config::guiConfigFilePath() const {
    static const auto guiConfigFilePath = util::configDir() / "gui.json";
    return guiConfigFilePath;
}

const std::filesystem::path& Config::apiConfigFilePath() const {
    static const auto apiConfigFilePath = util::configDir() / "api.json";
    return apiConfigFilePath;
}
