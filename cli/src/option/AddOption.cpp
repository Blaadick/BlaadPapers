// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/AddOption.hpp"

#include "../network/Url.hpp"

namespace fs = std::filesystem;

AddOption::AddOption(
    sptr<WallpaperLoaderManager> wallpaperLoader,
    sptr<HttpClient> httpClient,
    sptr<Config> config,
    sptr<util::Logger> logger
) : Option("Adds wallpaper(s) to the wallpapers folder"), wallpaperLoader(std::move(wallpaperLoader)), httpClient(std::move(httpClient)), config(std::move(config)), logger(std::move(logger)) {}

std::vector<std::string_view> AddOption::getUsageStrings() const {
    return {"<file/folder_path...>"};
}

int AddOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) {
    if(arguments.empty()) {
        logger->logWarning("One or more wallpaper file paths expected");
        return 1;
    }

    std::vector<fs::path> filePaths;
    std::vector<std::string_view> links;
    for(const auto& argument : arguments) {
        if(Url::isUrl(argument)) {
            links.emplace_back(argument);
            continue;
        }

        if(fs::is_regular_file(argument)) {
            filePaths.emplace_back(argument);
            continue;
        }

        logger->logWarning(std::format("Argument \"{}\" is not URL or file path", argument));
    }

    for(const auto& path : filePaths) {
        wallpaperLoader->addWallpaper(path, config->getWallpapersDirPath());
    }

    for(const auto& link : links) {
        auto downloadedFilePath = httpClient->downloadFile(link, util::localDataDir() / "downloads", "name.png");
        if(!downloadedFilePath.has_value()) {
            logger->logWarning(std::format("Failed to download file from {}", link));
            continue;
        }

        wallpaperLoader->addWallpaper(*downloadedFilePath, config->getWallpapersDirPath());
        fs::remove(*downloadedFilePath);
    }

    return 0;
}
