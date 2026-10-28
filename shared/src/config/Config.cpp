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
    constexpr std::array<std::string_view, 16> languages = {"system", "be", "de", "en_GB", "en_US", "es", "fr", "it", "ja", "ko", "pl", "pt_BR", "ru", "tr", "uk", "zh_CN"};

    auto validOrDefault(const std::string_view value, std::span<const std::string_view> allowed, const std::string_view fallback) -> std::string {
        return std::string(std::ranges::contains(allowed, value) ? value : fallback);
    }
}

Config::Config(sptr<util::Logger> logger, sptr<SecretStorage> secretStorage) : logger(std::move(logger)), secretStorage(std::move(secretStorage)) {}

void Config::load() {
    if(util::createDirIfNotExists(util::configDir())) {
        loadGeneral();
        loadGui();
    } else {
        logger->logError("Failed to create configs directory");
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

    auto postInstallScriptData = generalJson->tryGetString("post_install_script");
    if(postInstallScriptData.has_value()) {
        postInstallScript = *postInstallScriptData;
    }

    auto postApplyScriptData = generalJson->tryGetString("post_apply_script");
    if(postApplyScriptData.has_value()) {
        postApplyScript = *postApplyScriptData;
    }

    auto postDeleteScriptData = generalJson->tryGetString("post_delete_script");
    if(postDeleteScriptData.has_value()) {
        postDeleteScript = *postDeleteScriptData;
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

    auto nativeTextRenderingData = guiJson->tryGetBool("native_text_rendering");
    if(nativeTextRenderingData.has_value()) {
        isNativeTextRendering = *nativeTextRenderingData;
    }

    auto colorSchemeData = guiJson->tryGetString("color_scheme");
    if(colorSchemeData.has_value()) {
        colorScheme = validOrDefault(*colorSchemeData, colorSchemes, "system");
    }

    auto languageData = guiJson->tryGetString("language");
    if(languageData.has_value()) {
        language = validOrDefault(*languageData, languages, "system");
    }
}

void Config::save() const {
    saveGeneral();
    saveGui();
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
    yyjson_mut_obj_add_str(doc, root, "post_install_script", postInstallScript.c_str());
    yyjson_mut_obj_add_str(doc, root, "post_apply_script", postApplyScript.c_str());
    yyjson_mut_obj_add_str(doc, root, "post_delete_script", postDeleteScript.c_str());

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
    yyjson_mut_obj_add_bool(doc, root, "native_text_rendering", isNativeTextRendering);
    yyjson_mut_obj_add_str(doc, root, "color_scheme", colorScheme.c_str());
    yyjson_mut_obj_add_str(doc, root, "language", language.c_str());

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(guiConfigFilePath().string().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        logger->logError(std::format("Failed to write config file to \"{}\"", guiConfigFilePath()));
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

auto Config::getPostInstallScript() const noexcept -> const std::string& {
    return postInstallScript;
}

void Config::setPostInstallScript(std::string newScript) noexcept {
    postInstallScript = std::move(newScript);
    saveGeneral();
}

auto Config::getPostApplyScript() const noexcept -> const std::string& {
    return postApplyScript;
}

void Config::setPostApplyScript(std::string newScript) noexcept {
    postApplyScript = std::move(newScript);
    saveGeneral();
}

auto Config::getPostDeleteScript() const noexcept -> const std::string& {
    return postDeleteScript;
}

void Config::setPostDeleteScript(std::string newScript) noexcept {
    postDeleteScript = std::move(newScript);
    saveGeneral();
}

auto Config::getWallhavenApiKey() const -> std::expected<std::string, std::string> {
    return secretStorage->get("wallhaven/key");
}

void Config::setWallhavenApiKey(const std::string& newKey) const {
    auto result = secretStorage->set("wallhaven/key", newKey);
    if(!result.has_value()) {
        logger->logError(std::format("Failed to set wallhaven api key: {}", result.error()));
    }
}

auto Config::getDanbooruLogin() const -> std::expected<std::string, std::string> {
    return secretStorage->get("danbooru/login");
}

void Config::setDanbooruLogin(const std::string& newLogin) const {
    auto result = secretStorage->set("danbooru/login", newLogin);
    if(!result.has_value()) {
        logger->logError(std::format("Failed to set danbooru login: {}", result.error()));
    }
}

auto Config::getDanbooruApiKey() const -> std::expected<std::string, std::string> {
    return secretStorage->get("danbooru/key");
}

void Config::setDanbooruApiKey(const std::string& newKey) const {
    auto result = secretStorage->set("danbooru/key", newKey);
    if(!result.has_value()) {
        logger->logError(std::format("Failed to set danbooru api key: {}", result.error()));
    }
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

auto Config::getNativeTextRendering() const noexcept -> bool {
    return isNativeTextRendering;
}

void Config::setNativeTextRendering(const bool newNativeTextRendering) noexcept {
    isNativeTextRendering = newNativeTextRendering;
    saveGui();
}

auto Config::getColorScheme() const noexcept -> const std::string& {
    return colorScheme;
}

void Config::setColorScheme(std::string_view newColorScheme) noexcept {
    colorScheme = validOrDefault(newColorScheme, colorSchemes, "system");
    saveGui();
}

auto Config::getLanguage() const noexcept -> const std::string& {
    return language;
}

void Config::setLanguage(std::string_view newLanguage) noexcept {
    language = validOrDefault(newLanguage, languages, "system");
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
