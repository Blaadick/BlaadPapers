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

    if(!fs::exists(util::configDirPath())) {
        fs::create_directory(util::configDirPath());
    }

    if(fs::exists(configPath())) {
        std::ifstream configFile(configPath());
        configData = nlohmann::json::parse(configFile);

        if(configData.is_discarded()) {
            configData = defaultConfigData;
            std::cout << "Failed to parse config, fallback to default";
        }
    } else {
        std::ofstream configFile(configPath());
        configFile << defaultConfigData;
        configData = defaultConfigData;
    }

    badTags = configData["bad_tags"];
    wallpaperDirPaths = configData["wallpaper_paths"];
    isStatusBarVisible = configData["status_bar_visible"];
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
}

std::filesystem::path Config::configPath() {
    return util::configDirPath().append("config.json");
}

std::vector<std::string> Config::badTags;
std::vector<fs::path> Config::wallpaperDirPaths;
bool Config::isStatusBarVisible;
