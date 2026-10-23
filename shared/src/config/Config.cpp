// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config/Config.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <span>
#include <yyjson.h>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"

namespace {
    constexpr std::array<std::string_view, 3> colorSchemes = {"system", "dark", "light"};
    constexpr std::array<std::string_view, 3> contrasts = {"system", "normal", "high"};
    constexpr std::array<std::string_view, 3> motions = {"system", "normal", "reduced"};

    auto validOrDefault(const std::string_view value, std::span<const std::string_view> allowed) -> std::string {
        return std::string(std::ranges::contains(allowed, value) ? value : allowed.front());
    }
}

Config::Config(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void Config::load() {
    if(util::createDirIfNotExists(util::configDir())) {
        loadGeneral();
        loadGui();
        loadApi();
    } else {
        logger->logError("Failde to create configs directory");
    }
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

    auto defaultTagsData = generalJson->tryGetArr("default_tags");
    if(defaultTagsData.has_value()) {
        defaultTags.clear();

        defaultTagsData->forEachString(
            [this](std::string_view str) {
                defaultTags.emplace_back(str);
            }
        );
    }

    auto badTagsData = generalJson->tryGetArr("bad_tags");
    if(badTagsData.has_value()) {
        badTags.clear();

        badTagsData->forEachString(
            [this](std::string_view str) {
                badTags.emplace_back(str);
            }
        );
    }
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

    auto badTaggedWallpapersData = guiJson->tryGetBool("bad_tagged_wallpapers_visible");
    if(badTaggedWallpapersData.has_value()) {
        isBadTaggedWallpapersVisible = *badTaggedWallpapersData;
    }

    auto colorSchemeData = guiJson->tryGetString("color_scheme");
    if(colorSchemeData.has_value()) {
        colorScheme = validOrDefault(*colorSchemeData, colorSchemes);
    }

    auto contrastData = guiJson->tryGetString("contrast");
    if(contrastData.has_value()) {
        contrast = validOrDefault(*contrastData, contrasts);
    }

    auto motionData = guiJson->tryGetString("motion");
    if(motionData.has_value()) {
        motion = validOrDefault(*motionData, motions);
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

    auto wallhavenApiObj = apiJson->tryGetObj("wallhaven");
    if(wallhavenApiObj.has_value()) {
        auto wallhavenApiKeyStr = wallhavenApiObj->tryGetString("key");
        if(wallhavenApiKeyStr.has_value()) {
            wallhavenApiKey = *wallhavenApiKeyStr;
        }
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

    auto defaultTagsData = yyjson_mut_arr(doc);
    for(const auto& tag : defaultTags) {
        yyjson_mut_arr_add_str(doc, defaultTagsData, tag.c_str());
    }

    auto badTagsData = yyjson_mut_arr(doc);
    for(const auto& tag : badTags) {
        yyjson_mut_arr_add_str(doc, badTagsData, tag.c_str());
    }

    yyjson_mut_obj_add_strcpy(doc, root, "wallpapers_path", wallpapersDirPath.string().c_str());
    yyjson_mut_obj_add_val(doc, root, "default_tags", defaultTagsData);
    yyjson_mut_obj_add_val(doc, root, "bad_tags", badTagsData);

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(generalConfigFilePath().string().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError(std::format("Failed to write config file to \"{}\"", generalConfigFilePath()));
    }

    yyjson_mut_doc_free(doc);
}

void Config::saveGui() const {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    yyjson_mut_obj_add_bool(doc, root, "status_bar_visible", isStatusBarVisible);
    yyjson_mut_obj_add_bool(doc, root, "bad_tagged_wallpapers_visible", isBadTaggedWallpapersVisible);
    yyjson_mut_obj_add_str(doc, root, "color_scheme", colorScheme.c_str());
    yyjson_mut_obj_add_str(doc, root, "contrast", contrast.c_str());
    yyjson_mut_obj_add_str(doc, root, "motion", motion.c_str());

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(guiConfigFilePath().string().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError(std::format("Failed to write config file to \"{}\"", guiConfigFilePath()));
    }

    yyjson_mut_doc_free(doc);
}

void Config::saveApi() const {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    auto wallhavenData = yyjson_mut_obj_add_obj(doc, root, "wallhaven");
    if(wallhavenApiKey.has_value()) {
        yyjson_mut_obj_add_str(doc, wallhavenData, "key", wallhavenApiKey->c_str());
    } else {
        yyjson_mut_obj_add_null(doc, wallhavenData, "key");
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
    auto isWritten = yyjson_mut_write_file(apiConfigFilePath().string().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError(std::format("Failed to write config file to \"{}\"", apiConfigFilePath()));
    }

    yyjson_mut_doc_free(doc);
}

auto Config::getWallpapersDirPath() const noexcept -> const std::filesystem::path& {
    return wallpapersDirPath;
}

void Config::setWallpapersDirPath(std::filesystem::path newPath) noexcept {
    wallpapersDirPath = std::move(newPath);
    saveGeneral();
}

auto Config::getDefaultTags() const noexcept -> const std::vector<std::string>& {
    return defaultTags;
}

void Config::setDefaultTags(std::vector<std::string> newTags) noexcept {
    defaultTags = std::move(newTags);
    saveGeneral();
}

auto Config::getBadTags() const noexcept -> const std::vector<std::string>& {
    return badTags;
}

void Config::setBadTags(std::vector<std::string> newTags) noexcept {
    badTags = std::move(newTags);
    saveGeneral();
}

auto Config::isWallpaperBad(const Wallpaper& wallpaper) const noexcept -> bool {
    return std::ranges::any_of(
        badTags,
        [&wallpaper](const std::string& tag) {
            return std::ranges::contains(wallpaper.getTags(), tag);
        }
    );
}

auto Config::getWallhavenApiKey() const noexcept -> const std::optional<std::string>& {
    return wallhavenApiKey;
}

void Config::setWallhavenApiKey(std::optional<std::string> newKey) noexcept {
    wallhavenApiKey = std::move(newKey);
    saveApi();
}

auto Config::getDanbooruLogin() const noexcept -> const std::optional<std::string>& {
    return danbooruLogin;
}

void Config::setDanbooruLogin(std::optional<std::string> newLogin) noexcept {
    danbooruLogin = std::move(newLogin);
    saveApi();
}

auto Config::getDanbooruApiKey() const noexcept -> const std::optional<std::string>& {
    return danbooruApiKey;
}

void Config::setDanbooruApiKey(std::optional<std::string> newKey) noexcept {
    danbooruApiKey = std::move(newKey);
    saveApi();
}

auto Config::getStatusBarVisible() const noexcept -> bool {
    return isStatusBarVisible;
}

void Config::setStatusBarVisible(const bool newVisibility) noexcept {
    isStatusBarVisible = newVisibility;
    saveGui();
}

auto Config::getBadTaggedWallpapersVisible() const noexcept -> bool {
    return isBadTaggedWallpapersVisible;
}

void Config::setBadTaggedWallpapersVisible(const bool newVisibility) noexcept {
    isBadTaggedWallpapersVisible = newVisibility;
    saveGui();
}

auto Config::getColorScheme() const noexcept -> const std::string& {
    return colorScheme;
}

void Config::setColorScheme(std::string_view newColorScheme) noexcept {
    colorScheme = validOrDefault(newColorScheme, colorSchemes);
    saveGui();
}

auto Config::getContrast() const noexcept -> const std::string& {
    return contrast;
}

void Config::setContrast(std::string_view newContrast) noexcept {
    contrast = validOrDefault(newContrast, contrasts);
    saveGui();
}

auto Config::getMotion() const noexcept -> const std::string& {
    return motion;
}

void Config::setMotion(std::string_view newMotion) noexcept {
    motion = validOrDefault(newMotion, motions);
    saveGui();
}

auto Config::generalConfigFilePath() const noexcept -> const std::filesystem::path& {
    static const auto generalConfigFilePath = util::configDir() / "config.json";
    return generalConfigFilePath;
}

auto Config::guiConfigFilePath() const noexcept -> const std::filesystem::path& {
    static const auto guiConfigFilePath = util::configDir() / "gui.json";
    return guiConfigFilePath;
}

auto Config::apiConfigFilePath() const noexcept -> const std::filesystem::path& {
    static const auto apiConfigFilePath = util::configDir() / "api.json";
    return apiConfigFilePath;
}
