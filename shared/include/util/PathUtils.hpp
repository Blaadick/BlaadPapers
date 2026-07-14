// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <string>

namespace util {
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

    inline std::filesystem::path configFilePath() {
        return configDirPath().append("config.json");
    }

    inline std::filesystem::path localDataDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append(".local/share/blaadpapers");
        #endif
    }

    inline std::filesystem::path documentsDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append("Documents");
        #endif
    }

    inline std::filesystem::path currentWallpaperIdPath() {
        return localDataDirPath().append("current-wallpaper");
    }

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
}
