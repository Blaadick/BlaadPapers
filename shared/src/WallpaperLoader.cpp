// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "WallpaperLoader.hpp"

#include <fstream>
#include <iostream>
#include <print>
#include "Config.hpp"
#include "Wallpapers.hpp"
#include "util/Ffmpeg.hpp"
#include "util/FormatUtils.hpp"
#include "util/Vips.hpp"

namespace fs = std::filesystem;

void WallpaperLoader::loadWallpapers() {
    Wallpapers::inst().clear();
    jpegUnifier();

    vips_init("");

    for(const auto& wallpaperDirPath : Config::getWallpaperDirPaths()) {
        const auto wallpaperDataDirPath = wallpaperDirPath / ".index";

        if(!fs::exists(wallpaperDirPath)) {
            if(!fs::create_directory(wallpaperDirPath)) {
                std::println(stderr, "Cant create directory \"{}\"", wallpaperDirPath.c_str());
                continue;
            }
        }

        if(!fs::exists(wallpaperDataDirPath)) {
            if(!fs::create_directory(wallpaperDataDirPath)) {
                std::println(stderr, "Cant create directory \"{}\"", wallpaperDataDirPath.c_str());
                continue;
            }
        }

        for(const auto& entry : fs::recursive_directory_iterator(wallpaperDirPath)) {
            if(!entry.is_regular_file()) {
                continue;
            }

            const auto wallpaperId = entry.path().stem().string();
            const auto wallpaperDataPath = wallpaperDataDirPath / (wallpaperId + ".json");

            if(util::supportedPictureFormats.contains(entry.path().extension())) {
                Wallpapers::inst().add(
                    std::move(loadPictureWallpaper(entry.path(), readWallpaperData(wallpaperDataPath)))
                );
            }

            if(util::supportedVideoFormats.contains(entry.path().extension())) {
                Wallpapers::inst().add(
                    std::move(loadVideoWallpaper(entry.path(), readWallpaperData(wallpaperDataPath)))
                );
            }
        }

        Wallpapers::inst().sortByName();
    }

    vips_shutdown();
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

// TODO Move to std fs
uptr<PictureWallpaper> WallpaperLoader::loadPictureWallpaper(const std::filesystem::path& filePath, const nlohmann::json& data) {
    return std::make_unique<PictureWallpaper>(
        filePath.stem(),
        filePath,
        data["name"],
        getPictureResolutionData(filePath),
        data["source"],
        data["tags"]
    );
}

// TODO Move to std fs
uptr<VideoWallpaper> WallpaperLoader::loadVideoWallpaper(const std::filesystem::path& filePath, const nlohmann::json& data) {
    auto [resolution, frameRate] = getVideoData(filePath);

    return std::make_unique<VideoWallpaper>(
        filePath.stem(),
        filePath,
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
