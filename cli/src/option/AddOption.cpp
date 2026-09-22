// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "option/AddOption.hpp"

#include <format>

namespace fs = std::filesystem;

AddOption::AddOption(
    sptr<WallpaperLoaderManager> wallpaperLoader,
    sptr<DownloadManager> downloadManager,
    sptr<Config> config,
    sptr<util::Logger> logger
) : Option("Adds wallpaper(s) to the wallpapers folder"), wallpaperLoader(std::move(wallpaperLoader)), downloadManager(std::move(downloadManager)), config(std::move(config)), logger(std::move(logger)) {}

auto AddOption::getUsageStrings() const noexcept -> std::vector<std::string_view> {
    return {"<file/URI...>"};
}

auto AddOption::execute(
    const std::vector<std::string_view>& arguments,
    const std::unordered_set<sptr<Flag>>& flags
) -> int {
    if(arguments.empty()) {
        logger->logWarning("One or more URI expected");
        return 1;
    }

    std::vector<fs::path> filePaths;
    std::vector<Uri> uris;

    for(const auto& argument : arguments) {
        if(Uri::isUri(argument)) {
            uris.emplace_back(argument);
        } else {
            filePaths.emplace_back(argument);
        }
    }

    for(const auto& path : filePaths) {
        auto wallpaper = wallpaperLoader->installWallpaper(path);
        if(!wallpaper.has_value()) {
            logger->logWarning(std::format("Failed to install \"{}\": {}", path, wallpaper.error()));
        }
    }

    for(const auto& url : uris) {
        logger->logInfo(std::format("Downloading from \"{}\"...", url));

        auto downloadedFilePath = downloadManager->downloadFile(url, util::localDownloadsDirPath());
        if(!downloadedFilePath.has_value()) {
            logger->logWarning(std::format("Failed to download file from \"{}\": {}", url, downloadedFilePath.error()));
            continue;
        }

        auto wallpaper = wallpaperLoader->installWallpaper(*downloadedFilePath);
        if(!wallpaper.has_value()) {
            logger->logWarning(std::format("Failed to install \"{}\": {}", *downloadedFilePath, wallpaper.error()));
        }

        fs::remove(*downloadedFilePath);
    }

    return 0;
}
