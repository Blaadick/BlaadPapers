// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/AddOption.hpp"

#include <format>
#include "util/BoostUriUtils.hpp"

namespace fs = std::filesystem;

AddOption::AddOption(
    sptr<WallpaperLoaderManager> wallpaperLoader,
    sptr<HttpClient> httpClient,
    sptr<Config> config,
    sptr<util::Logger> logger
) : Option("Adds wallpaper(s) to the wallpapers folder"), wallpaperLoader(std::move(wallpaperLoader)), httpClient(std::move(httpClient)), config(std::move(config)), logger(std::move(logger)) {}

std::vector<std::string_view> AddOption::getUsageStrings() const {
    return {"<file/URI...>"};
}

int AddOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) {
    if(arguments.empty()) {
        logger->logWarning("One or more URI expected");
        return 1;
    }

    std::vector<fs::path> filePaths;
    std::vector<boost::url_view> urls;

    // TODO Make WallpaperInstallerDispatcher
    for(const auto& argument : arguments) {
        auto uri = boost::urls::parse_uri(argument);
        if(!uri.has_value()) {
            filePaths.emplace_back(argument);
            continue;
        }

        switch(uri->scheme_id()) {
            case boost::urls::scheme::file: {
                filePaths.emplace_back(uri->path());
                break;
            }

            case boost::urls::scheme::http:
            case boost::urls::scheme::https: {
                urls.emplace_back(*uri);
                break;
            }

            default: {
                logger->logWarning(std::format("URI \"{}\" is unsupported", *uri));
                break;
            }
        }
    }

    for(const auto& path : filePaths) {
        wallpaperLoader->addWallpaper(path, config->getWallpapersDirPath());
    }

    for(const auto& url : urls) {
        auto downloadedFilePath = httpClient->downloadFile(url, util::localDataDir() / "downloads");
        if(!downloadedFilePath.has_value()) {
            logger->logWarning(std::format("Failed to download file from \"{}\": {}", url, downloadedFilePath.error()));
            continue;
        }

        wallpaperLoader->addWallpaper(*downloadedFilePath, config->getWallpapersDirPath());
        fs::remove(*downloadedFilePath);
    }

    return 0;
}
