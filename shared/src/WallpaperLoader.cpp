// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "WallpaperLoader.hpp"

#include <fstream>
#include "Config.hpp"
#include "Wallpapers.hpp"
#include "util/Ffmpeg.hpp"
#include "util/ImageUtils.hpp"
#include "util/PathUtils.hpp"
#include "util/WallpaperUtils.hpp"

namespace fs = std::filesystem;

WallpaperLoader::WallpaperLoader(
    sptr<Wallpapers> wallpapers,
    sptr<Config> config,
    sptr<util::Logger> logger
) : wallpapers(std::move(wallpapers)), config(std::move(config)), logger(std::move(logger)) {}

void WallpaperLoader::loadWallpapers() {
    wallpapers->clear();

    if(!util::createDirIfNotExists(config->getWallpapersDirPath())) {
        logger->logError("Failed to create directory \"" + config->getWallpapersDirPath().string() + "\"");
        return;
    }

    for(const auto& wallpapersDirEntry : fs::directory_iterator(config->getWallpapersDirPath())) {
        if(!wallpapersDirEntry.is_directory()) {
            continue;
        }

        for(const auto& wallpaperDirEntry : fs::directory_iterator(wallpapersDirEntry.path())) {
            if(wallpaperDirEntry.path().stem() != "wallpaper") {
                continue;
            }

            if(util::isSupportedPicture(wallpaperDirEntry.path())) {
                wallpapers->add(
                    loadPictureWallpaper(
                        wallpapersDirEntry.path().stem().string(),
                        wallpaperDirEntry.path(),
                        readWallpaperData(wallpapersDirEntry.path() / "data.json")
                    )
                );
                break;
            }

            if(util::isSupportedVideo(wallpaperDirEntry.path())) {
                wallpapers->add(
                    loadVideoWallpaper(
                        wallpapersDirEntry.path().stem().string(),
                        wallpaperDirEntry.path(),
                        readWallpaperData(wallpapersDirEntry.path() / "data.json")
                    )
                );
                break;
            }
        }
    }
}

bool WallpaperLoader::addWallpaper(
    const fs::path& wallpaperFilePath,
    const fs::path& destinationFolderPath
) {
    const auto wallpaperId = wallpaperFilePath.stem().string();
    const auto wallpaperRootPath = destinationFolderPath / wallpaperId;
    const auto isPicture = util::isSupportedPicture(wallpaperFilePath);
    const auto isVideo = util::isSupportedVideo(wallpaperFilePath);

    if(!isPicture && !isVideo) {
        logger->logWarning("Failed to add \"" + wallpaperId + "\" wallpaper: \"" + wallpaperFilePath.extension().string() + "\" files are not supported");
        return false;
    }

    if(fs::exists(wallpaperRootPath)) {
        logger->logWarning("Failed to add \"" + wallpaperId + "\" wallpaper: Wallpaper with same id already exists");
        return false;
    }

    if(!fs::create_directory(wallpaperRootPath)) {
        logger->logWarning("Failed to add \"" + wallpaperId + "\" wallpaper: Failed to create directory \"" + wallpaperRootPath.string() + "\"");
        return false;
    }

    const auto newWallpaperFilePath = wallpaperRootPath / ("wallpaper" + wallpaperFilePath.extension().string());
    if(!fs::copy_file(wallpaperFilePath, newWallpaperFilePath)) {
        logger->logWarning("Failed to add \"" + wallpaperId + "\" wallpaper: Failed to copy wallpaper to \"" + newWallpaperFilePath.string() + "\"");
        fs::remove_all(wallpaperRootPath);
        return false;
    }

    if(isPicture) {
        wallpapers->add(
            std::move(
                loadPictureWallpaper(
                    wallpaperId,
                    newWallpaperFilePath,
                    readWallpaperData(wallpaperRootPath / "data.json")
                )
            )
        );
        return true;
    }

    if(isVideo) {
        wallpapers->add(
            std::move(
                loadVideoWallpaper(
                    wallpaperId,
                    newWallpaperFilePath,
                    readWallpaperData(wallpaperRootPath / "data.json")
                )
            )
        );
        return true;
    }

    return false;
}

// TODO Refactor
void WallpaperLoader::addWallpapers(
    const std::vector<fs::path>& paths,
    const fs::path& destinationFolderPath
) {
    for(const auto& path : paths) {
        if(fs::is_directory(path)) {
            for(const auto& dirEntry : fs::directory_iterator(path)) {
                if(!dirEntry.is_regular_file()) {
                    continue;
                }

                addWallpaper(dirEntry.path(), destinationFolderPath);
            }

            continue;
        }

        if(fs::is_regular_file(path)) {
            addWallpaper(path, destinationFolderPath);
        }
    }
}

// TODO Split it away
nlohmann::json WallpaperLoader::readWallpaperData(const std::filesystem::path& wallpaperDataPath) {
    nlohmann::json defaultWallpaperData = {
        {"name", wallpaperDataPath.parent_path().stem()},
        {"source", ""},
        {"tags", {"General"}}
    };
    nlohmann::json wallpaperData;
    bool isWallpaperDataFull;

    if(fs::exists(wallpaperDataPath)) {
        isWallpaperDataFull = true;

        std::ifstream wallpaperDataFile(wallpaperDataPath);
        wallpaperData = nlohmann::json::parse(wallpaperDataFile);

        if(wallpaperData.is_discarded()) {
            isWallpaperDataFull = false;
        } else {
            if(wallpaperData["name"].is_null()) {
                wallpaperData["name"] = defaultWallpaperData["name"];
                isWallpaperDataFull = false;
            }

            if(wallpaperData["source"].is_null()) {
                wallpaperData["source"] = defaultWallpaperData["source"];
                isWallpaperDataFull = false;
            }

            if(wallpaperData["tags"].is_null()) {
                wallpaperData["tags"] = defaultWallpaperData["tags"];
                isWallpaperDataFull = false;
            }
        }
    } else {
        isWallpaperDataFull = false;
        wallpaperData = defaultWallpaperData;
    }

    if(!isWallpaperDataFull) {
        std::ofstream wallpaperDataFile(wallpaperDataPath);
        wallpaperDataFile << wallpaperData.dump(4);
    }

    return wallpaperData;
}

uptr<PictureWallpaper> WallpaperLoader::loadPictureWallpaper(
    const std::string& wallpaperId,
    const std::filesystem::path& filePath,
    const nlohmann::json& data
) {
    return std::make_unique<PictureWallpaper>(
        wallpaperId,
        filePath,
        filePath.parent_path(),
        data["name"],
        getPictureResolutionData(filePath),
        data["source"],
        data["tags"]
    );
}

uptr<VideoWallpaper> WallpaperLoader::loadVideoWallpaper(
    const std::string& wallpaperId,
    const std::filesystem::path& filePath,
    const nlohmann::json& data
) {
    auto [resolution, frameRate] = getVideoData(filePath);

    return std::make_unique<VideoWallpaper>(
        wallpaperId,
        filePath,
        filePath.parent_path(),
        data["name"],
        resolution,
        frameRate,
        data["source"],
        data["tags"]
    );
}
