// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "config/Config.hpp"

#include <algorithm>
#include <fstream>
#include <yyjson.h>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"

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

    auto generalJson = JsonObj::tryParse(generalConfigFilePath());
    if(!generalJson.has_value()) {
        logger->logError(std::format("Failed to parse general config file: {}", generalJson.error()));
        return;
    }

    auto wallpapersPathData = generalJson->tryGetString("wallpapers_path");
    if(wallpapersPathData.has_value()) {
        wallpapersDirPath = *wallpapersPathData;
    }

    auto badTagsData = generalJson->tryGetArr("bad_tags");
    badTagsData->forEachString(
        [this](std::string_view str) {
            badTags.emplace_back(str);
        }
    );
}

void Config::loadGui() {
    if(!std::filesystem::exists(guiConfigFilePath())) {
        saveGui();
        return;
    }

    auto guiJson = JsonObj::tryParse(guiConfigFilePath());
    if(!guiJson.has_value()) {
        logger->logError(std::format("Failed to parse GUI config file: {}", guiJson.error()));
        return;
    }

    auto statusBarVisibleData = guiJson->tryGetBool("status_bar_visible");
    if(statusBarVisibleData.has_value()) {
        isStatusBarVisible = *statusBarVisibleData;
    }
}

void Config::loadApi() {
    if(!std::filesystem::exists(apiConfigFilePath())) {
        saveApi();
        return;
    }

    auto apiJson = JsonObj::tryParse(apiConfigFilePath());
    if(!apiJson.has_value()) {
        logger->logError(std::format("Failed to parse API config file: {}", apiJson.error()));
        return;
    }

    auto wallhavenApiStr = apiJson->tryGetString("wallhaven");
    if(wallhavenApiStr.has_value()) {
        wallhavenApiKey = *wallhavenApiStr;
    }

    auto danbooruApiObj = apiJson->tryGetObj("danbooru");
    if(danbooruApiObj.has_value()) {
        auto danbooruLoginStr = danbooruApiObj->tryGetString("login");
        if(danbooruLoginStr.has_value()) {
            danbooruLogin = *danbooruLoginStr;
        }

        auto danbooruApiKeyStr = danbooruApiObj->tryGetString("key");
        if(danbooruApiKeyStr.has_value()) {
            danbooruApiKey = *danbooruApiKeyStr;
        }
    }
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
