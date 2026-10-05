// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "option/InstallOption.hpp"

#include <format>

namespace fs = std::filesystem;

InstallOption::InstallOption(
    sptr<WallpaperLoaderManager> wallpaperLoader,
    sptr<DownloadManager> downloadManager,
    sptr<Config> config,
    sptr<util::Logger> logger
) : Option("Installs file(s) to the wallpapers folder; Downloads file(s) from internet and install them"), wallpaperLoader(std::move(wallpaperLoader)), downloadManager(std::move(downloadManager)), config(std::move(config)), logger(std::move(logger)) {}

auto InstallOption::getUsageStrings() const noexcept -> std::vector<std::string_view> {
    return {"<file/URI...>", "./Downloads/some_file.ext https://example.com/some_file.ext"};
}

auto InstallOption::execute(
    const std::vector<std::string_view>& arguments,
    const std::unordered_set<sptr<Flag>>& flags
) -> int {
    if(arguments.empty()) {
        logger->logWarning("One or more URI expected");
        return 1;
    }

    std::vector<fs::path> filePaths;
    std::vector<Uri> uris;

    for(auto& argument : arguments) {
        auto uri = Uri::parse(std::string(argument));
        if(uri.has_value()) {
            uris.emplace_back(std::move(*uri));
        } else {
            if(std::filesystem::exists(argument)) {
                filePaths.emplace_back(argument);
            } else {
                logger->logWarning(std::format("File \"{}\" does not exists", argument));
            }
        }
    }

    for(auto& path : filePaths) {
        auto wallpaper = wallpaperLoader->installWallpaper(path);
        if(!wallpaper.has_value()) {
            logger->logWarning(std::format("Failed to install \"{}\": {}", path.filename(), wallpaper.error()));
        }
    }

    for(auto& uri : uris) {
        logger->logInfo(std::format("Downloading from \"{}\"...", uri));

        auto downloadedFilePath = downloadManager->downloadFile(uri, util::localDownloadsDirPath());
        if(!downloadedFilePath.has_value()) {
            logger->logWarning(std::format("Failed to download file from \"{}\": {}", uri, downloadedFilePath.error()));
            continue;
        }

        auto wallpaper = wallpaperLoader->installWallpaper(*downloadedFilePath);
        if(!wallpaper.has_value()) {
            logger->logWarning(std::format("Failed to install \"{}\": {}", downloadedFilePath->filename(), wallpaper.error()));
        }

        fs::remove(*downloadedFilePath);
    }

    return 0;
}
