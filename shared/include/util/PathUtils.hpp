// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>

namespace util {
    inline std::filesystem::path cacheDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append(".cache/blaadpapers");
        #elif _WIN32
        std::filesystem::path localAppData = getenv("LOCALAPPDATA");
        return localAppData.append("blaadpapers").append("cache");
        #endif
    }

    inline std::filesystem::path configDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append(".config/blaadpapers");
        #elif _WIN32
        std::filesystem::path appData = getenv("APPDATA");
        return appData.append("blaadpapers").append("config");
        #endif
    }

    inline std::filesystem::path generalConfigFilePath() {
        return configDirPath().append("config.json");
    }

    inline std::filesystem::path guiConfigFilePath() {
        return configDirPath().append("gui.json");
    }

    inline std::filesystem::path apiConfigFilePath() {
        return configDirPath().append("api.json");
    }

    inline std::filesystem::path localDataDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append(".local/share/blaadpapers");
        #elif _WIN32
        std::filesystem::path localAppData = getenv("LOCALAPPDATA");
        return localAppData.append("blaadpapers");
        #endif
    }

    inline std::filesystem::path documentsDirPath() {
        #ifdef __linux__
        std::filesystem::path homeDir = getenv("HOME");
        return homeDir.append("Documents");
        #elif _WIN32
        std::filesystem::path userProfile = getenv("USERPROFILE");
        return userProfile.append("Documents");
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
            if(!std::filesystem::create_directories(path)) {
                return false;
            }
        }

        return true;
    }
}
