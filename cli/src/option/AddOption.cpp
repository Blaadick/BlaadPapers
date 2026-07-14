// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/AddOption.hpp"

namespace fs = std::filesystem;

AddOption::AddOption(
    sptr<WallpaperLoader> wallpaperLoader,
    sptr<Config> config,
    sptr<util::Logger> logger
) : wallpaperLoader(std::move(wallpaperLoader)), config(std::move(config)), logger(std::move(logger)) {}

std::string AddOption::getHelpMessage() const {
    return "Add help";
}

int AddOption::execute(const std::vector<std::string>& arguments) {
    if(arguments.empty()) {
        logger->logWarning("One or more wallpaper file paths expected");
        return 1;
    }

    std::vector<fs::path> wallpapersToAddPaths;
    for(const auto rawWallpaperFilePath : arguments) {
        wallpapersToAddPaths.emplace_back(rawWallpaperFilePath);
    }

    wallpaperLoader->addWallpapers(wallpapersToAddPaths, config->getWallpaperDirPaths()[0]);
    return 0;
}
