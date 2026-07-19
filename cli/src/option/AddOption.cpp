// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/AddOption.hpp"

namespace fs = std::filesystem;

AddOption::AddOption(
    sptr<WallpaperLoader> wallpaperLoader,
    sptr<Config> config,
    sptr<util::Logger> logger
) : Option("Adds wallpaper(s) to the first wallpapers folder"), wallpaperLoader(std::move(wallpaperLoader)), config(std::move(config)), logger(std::move(logger)) {}

std::vector<std::string_view> AddOption::getUsageStrings() const {
    return {"<file/folder_path...>"};
}

int AddOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) {
    if(arguments.empty()) {
        logger->logWarning("One or more wallpaper file paths expected");
        return 1;
    }

    std::vector<fs::path> wallpapersToAddPaths;
    for(const auto rawWallpaperFilePath : arguments) {
        wallpapersToAddPaths.emplace_back(rawWallpaperFilePath);
    }

    wallpaperLoader->addWallpapers(wallpapersToAddPaths, config->getWallpapersDirPath());
    return 0;
}
