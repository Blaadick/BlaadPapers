// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <string>

namespace util {
    /**
     * Returns false on error. If directory already exists, or was successfully created, returns true.
     */
    inline bool createDirIfNotExists(const std::filesystem::path& path) {
        if(!std::filesystem::exists(path)) {
            if(!std::filesystem::create_directory(path)) {
                return false;
            }
        }

        return true;
    }

    inline std::filesystem::path cacheDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append(".cache/blaadpapers");
        #endif
    }

    inline std::filesystem::path configDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append(".config/blaadpapers");
        #endif
    }

    inline std::filesystem::path localDataDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append(".local/share/blaadpapers");
        #endif
    }

    inline std::filesystem::path picturesDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append("Pictures");
        #endif
    }

    inline std::filesystem::path videosDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append("Videos");
        #endif
    }

    //TODO Move this shit to gui
    inline std::filesystem::path previewsDirPath() {
        return cacheDirPath().append("preview");
    }

    inline std::filesystem::path currentWallpaperIdPath() {
        return localDataDirPath().append("current-wallpaper");
    }

    inline std::filesystem::path defaultWallpaperPath() {
        return localDataDirPath().append("default-wallpaper.png");
    }
}
