// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "WallpaperLoader.hpp"

#include <fstream>
#include <iostream>
#include <print>
#include "Config.hpp"
#include "Wallpapers.hpp"
#include "util/Ffmpeg.hpp"
#include "util/PathUtils.hpp"
#include "util/Vips.hpp"

namespace fs = std::filesystem;

void WallpaperLoader::loadWallpapers() {
    Wallpapers::inst().clear();
    jpegUnifier();

    for(const auto& wallpapersDirPath : Config::getWallpaperDirPaths()) {
        if(!util::createDirIfNotExists(wallpapersDirPath)) {
            std::println(stderr, "Failed to create directory \"{}\"", wallpapersDirPath.c_str());
            continue;
        }

        for(const auto& wallpapersDirEntry : fs::directory_iterator(wallpapersDirPath)) {
            if(!wallpapersDirEntry.is_directory()) {
                continue;
            }

            for(const auto& wallpaperDirEntry : fs::directory_iterator(wallpapersDirEntry.path())) {
                if(wallpaperDirEntry.path().stem() != "wallpaper") {
                    continue;
                }

                if(PictureWallpaper::supportedFormats.contains(wallpaperDirEntry.path().extension())) {
                    Wallpapers::inst().add(
                        std::move(
                            loadPictureWallpaper(
                                wallpapersDirEntry.path().stem(),
                                wallpaperDirEntry.path(),
                                readWallpaperData(wallpapersDirEntry.path() / "data.json")
                            )
                        )
                    );
                    break;
                }

                if(VideoWallpaper::supportedFormats.contains(wallpaperDirEntry.path().extension())) {
                    Wallpapers::inst().add(
                        std::move(
                            loadVideoWallpaper(
                                wallpapersDirEntry.path().stem(),
                                wallpaperDirEntry.path(),
                                readWallpaperData(wallpapersDirEntry.path() / "data.json")
                            )
                        )
                    );
                    break;
                }
            }
        }
    }

    Wallpapers::inst().sortByName();
}

// TODO Split it away
nlohmann::json WallpaperLoader::readWallpaperData(const std::filesystem::path& wallpaperDataPath) {
    nlohmann::json defaultWallpaperData = {
        {"name", wallpaperDataPath.stem()},
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

void WallpaperLoader::jpegUnifier() {
    const std::pmr::unordered_set<std::string> wrongVariants = {".jpg", ".JPG", ".jpe", ".jif", ".jfi", ".jfif"};

    for(const auto& wallpaperDirPath : Config::getWallpaperDirPaths()) {
        for(const auto& entry : fs::recursive_directory_iterator(wallpaperDirPath)) {
            if(!entry.path().has_extension() || !wrongVariants.contains(entry.path().extension())) {
                continue;
            }

            auto newPath = entry.path();
            newPath.replace_extension(".jpeg");

            fs::rename(entry.path(), newPath);
        }
    }
}
