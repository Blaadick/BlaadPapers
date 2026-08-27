// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>

namespace util {
    inline const std::filesystem::path& userHomeDir() {
        #ifdef __linux__
        static const auto homeDir = std::filesystem::path(std::getenv("HOME"));
        #elif _WIN32
        static const auto homeDir = std::filesystem::path(std::getenv("USERPROFILE"));
        #endif

        return homeDir;
    }

    #ifdef _WIN32
    inline const std::filesystem::path& appDataDir() {
        static const auto appDataDir = std::filesystem::path(std::getenv("APPDATA"));
        return appDataDir;
    }

    inline const std::filesystem::path& localAppDataDir() {
        static const auto localAppDataDir = std::filesystem::path(std::getenv("LOCALAPPDATA"));
        return localAppDataDir;
    }
    #endif

    inline const std::filesystem::path& desktopDir() {
        static const auto desktopDir = userHomeDir() / "Desktop";
        return desktopDir;
    }

    inline const std::filesystem::path& documentsDir() {
        static const auto documentsDir = userHomeDir() / "Documents";
        return documentsDir;
    }

    inline const std::filesystem::path& downloadsDir() {
        static const auto downloadsDir = userHomeDir() / "Downloads";
        return downloadsDir;
    }

    inline const std::filesystem::path& musicDir() {
        static const auto musicDir = userHomeDir() / "Music";
        return musicDir;
    }

    inline const std::filesystem::path& picturesDir() {
        static const auto picturesDir = userHomeDir() / "Pictures";
        return picturesDir;
    }

    inline const std::filesystem::path& videosDir() {
        static const auto videosDir = userHomeDir() / "Videos";
        return videosDir;
    }

    inline const std::filesystem::path& cacheDir() {
        #ifdef __linux__
        static const auto cacheDir = userHomeDir() / ".cache" / "blaadpapers";
        #elif _WIN32
        static const auto cacheDir = localAppDataDir() / "blaadpapers" / "cache";
        #endif

        return cacheDir;
    }

    inline const std::filesystem::path& configDir() {
        #ifdef __linux__
        static const auto configDir = userHomeDir() / ".config" / "blaadpapers";
        #elif _WIN32
        static const auto configDir = appDataDir() / "blaadpapers";
        #endif

        return configDir;
    }

    inline const std::filesystem::path& localDataDir() {
        #ifdef __linux__
        static const auto localDataDirPath = userHomeDir() / ".local" / "share" / "blaadpapers";
        #elif _WIN32
        static const auto localDataDirPath = localAppDataDir() / "blaadpapers";
        #endif

        return localDataDirPath;
    }

    inline const std::filesystem::path& currentWallpaperIdFilePath() {
        static auto const currentWallpaperIdFilePath = localDataDir() / "current-wallpaper";
        return currentWallpaperIdFilePath;
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
