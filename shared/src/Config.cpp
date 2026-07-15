// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;
namespace rng = std::ranges;

Config::Config(sptr<util::Logger> logger): logger(std::move(logger)) {}

void Config::load() {
    nlohmann::json defaultConfigData = {
        {"bad_tags", {"Sensitive", "Questionable", "Explicit"}},
        {"wallpaper_paths", {util::documentsDirPath().append("Wallpapers")}},
        {"status_bar_visible", false}
    };
    nlohmann::json configData;

    util::createDirIfNotExists(util::configDirPath());

    if(fs::exists(util::configFilePath())) {
        std::ifstream configFile(util::configFilePath());
        configData = nlohmann::json::parse(configFile);

        if(configData.is_discarded()) {
            configData = defaultConfigData;
            logger->logWarning("Failed to parse config, fallback to default");
        }
    } else {
        std::ofstream configFile(util::configFilePath());
        configFile << defaultConfigData.dump(4);
        configData = defaultConfigData;
    }

    badTags = configData["bad_tags"].is_null() ? defaultConfigData["bad_tags"] : configData["bad_tags"];
    wallpaperDirPaths = configData["wallpaper_paths"].is_null() ? defaultConfigData["wallpaper_paths"] : configData["wallpaper_paths"];
    isStatusBarVisible = configData["status_bar_visible"].is_null() ? defaultConfigData["status_bar_visible"] : configData["status_bar_visible"];
}

std::vector<std::string> Config::getBadTags() {
    return badTags;
}

std::vector<fs::path> Config::getWallpaperDirPaths() {
    return wallpaperDirPaths;
}

bool Config::getStatusBarVisible() const {
    return isStatusBarVisible;
}

void Config::setStatusBarVisible(const bool newVisibility) {
    isStatusBarVisible = newVisibility;
    updateConfig("status_bar_visible", isStatusBarVisible);
}

bool Config::isWallpaperBad(const Wallpaper& wallpaper) {
    return rng::any_of(
        badTags,
        [&wallpaper](const std::string& tag) {
            return rng::contains(wallpaper.getTags(), tag);
        }
    );
}

template<typename T>
void Config::updateConfig(const std::string& settingName, const T& value) {
    nlohmann::json configData;

    {
        std::ifstream configFile(util::configFilePath());
        configFile >> configData;
    }

    configData[settingName] = value;

    {
        std::ofstream configFile(util::configFilePath());
        configFile << configData.dump(4).append("\n");
    }
}
