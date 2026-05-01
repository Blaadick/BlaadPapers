// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Config.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

void Config::load() {
    nlohmann::json defaultConfigData = {
        {"bad_tags", {"Sensitive", "Questionable", "Explicit"}},
        {"wallpaper_paths", {util::picturesDirPath().append("Wallpapers")}},
        {"status_bar_visible", false}
    };
    nlohmann::json configData;

    util::createDirIfNotExists(util::configDirPath());

    if(fs::exists(configPath())) {
        std::ifstream configFile(configPath());
        configData = nlohmann::json::parse(configFile);

        if(configData.is_discarded()) {
            configData = defaultConfigData;
            std::cout << "Failed to parse config, fallback to default";
        }
    } else {
        std::ofstream configFile(configPath());
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

bool Config::getStatusBarVisible() {
    return isStatusBarVisible;
}

void Config::setStatusBarVisible(const bool newVisibility) {
    isStatusBarVisible = newVisibility;
    updateConfig("status_bar_visible", isStatusBarVisible);
}

std::filesystem::path Config::configPath() {
    return util::configDirPath().append("config.json");
}

std::vector<std::string> Config::badTags;
std::vector<fs::path> Config::wallpaperDirPaths;
bool Config::isStatusBarVisible;

template<typename T>
void Config::updateConfig(const std::string& name, const T& value) {
    nlohmann::json configData;

    {
        std::ifstream configFile(configPath());
        configFile >> configData;
    }

    configData[name] = value;

    {
        std::ofstream configFile(configPath());
        configFile << configData.dump(4).append("\n");
    }
}
