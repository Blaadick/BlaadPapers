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
    formatUnifier();

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
}

bool WallpaperLoader::addWallpaper(
    const fs::path& wallpaperFilePath,
    const fs::path& destinationFolderPath
) {
    bool isPicture = false;
    if(PictureWallpaper::supportedFormats.contains(wallpaperFilePath.extension())) {
        isPicture = true;
    }

    bool isVideo = false;
    if(VideoWallpaper::supportedFormats.contains(wallpaperFilePath.extension())) {
        isVideo = true;
    }

    if(!isVideo && !isPicture) {
        std::println(stderr, "\"{}\" files are not supported", wallpaperFilePath.extension().c_str());
        return false;
    }

    const auto wallpaperRootPath = destinationFolderPath / wallpaperFilePath.stem();
    if(fs::exists(wallpaperRootPath)) {
        std::println(stderr, "Wallpaper with id \"{}\" already exists", wallpaperFilePath.stem().c_str());
        return false;
    }

    if(!fs::create_directory(wallpaperRootPath)) {
        std::println(stderr, "Failed to create directory \"{}\"", wallpaperRootPath.c_str());
        return false;
    }

    const auto newWallpaperFilePath = wallpaperRootPath / ("wallpaper" + wallpaperFilePath.extension().string());
    if(!fs::copy_file(wallpaperFilePath, newWallpaperFilePath)) {
        fs::remove_all(wallpaperRootPath);
        return false;
    }

    if(isPicture) {
        Wallpapers::inst().add(
            std::move(
                loadPictureWallpaper(
                    wallpaperFilePath.stem(),
                    newWallpaperFilePath,
                    readWallpaperData(wallpaperRootPath / "data.json")
                )
            )
        );
        return true;
    }

    if(isVideo) {
        Wallpapers::inst().add(
            std::move(
                loadVideoWallpaper(
                    wallpaperFilePath.stem(),
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

                if(!addWallpaper(dirEntry.path(), destinationFolderPath)) {
                    std::println(stderr, "Failed to add wallpaper from file \"{}\"", dirEntry.path().c_str());
                }
            }

            continue;
        }

        if(fs::is_regular_file(path)) {
            if(!addWallpaper(path, destinationFolderPath)) {
                std::println(stderr, "Failed to add wallpaper from file \"{}\"", path.c_str());
            }
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

void WallpaperLoader::formatUnifier() {
    const std::pmr::unordered_set<std::string> wrongJpegVariants = {".jpg", ".JPG", ".jpe", ".jif", ".jfi", ".jfif"};
    const std::pmr::unordered_set<std::string> wrongTiffVariants = {".tif"};
    const std::pmr::unordered_set<std::string> wrongHeicVariants = {".heif", ".hif", ".avic"};
    const std::pmr::unordered_set<std::string> wrongHeicsVariants = {".heifs", ".avcs"};

    for(const auto& wallpapersDirPath : Config::getWallpaperDirPaths()) {
        if(!util::createDirIfNotExists(wallpapersDirPath)) {
            std::println(stderr, "Failed to create directory \"{}\"", wallpapersDirPath.c_str());
            continue;
        }

        for(const auto& entry : fs::recursive_directory_iterator(wallpapersDirPath)) {
            if(!entry.path().has_extension()) {
                continue;
            }

            auto newPath = entry.path();

            if(wrongJpegVariants.contains(entry.path().extension())) {
                newPath.replace_extension(".jpeg");
                fs::rename(entry.path(), newPath);
                continue;
            }

            if(wrongTiffVariants.contains(entry.path().extension())) {
                newPath.replace_extension(".tiff");
                fs::rename(entry.path(), newPath);
                continue;
            }

            if(wrongHeicVariants.contains(entry.path().extension())) {
                newPath.replace_extension(".heic");
                fs::rename(entry.path(), newPath);
                continue;
            }

            if(wrongHeicsVariants.contains(entry.path().extension())) {
                newPath.replace_extension(".heics");
                fs::rename(entry.path(), newPath);
                continue;
            }
        }
    }
}
