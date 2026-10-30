// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>

namespace util {
    inline auto userHomeDir() -> const std::filesystem::path& {
        #ifdef __linux__
        static const auto homeDir = std::filesystem::path(std::getenv("HOME"));
        #elif _WIN32
        static const auto homeDir = std::filesystem::path(std::getenv("USERPROFILE"));
        #endif

        return homeDir;
    }

    #ifdef _WIN32
    inline auto appDataDir() -> const std::filesystem::path& {
        static const auto appDataDir = std::filesystem::path(std::getenv("APPDATA"));
        return appDataDir;
    }

    inline auto localAppDataDir() -> const std::filesystem::path& {
        static const auto localAppDataDir = std::filesystem::path(std::getenv("LOCALAPPDATA"));
        return localAppDataDir;
    }
    #endif

    inline auto desktopDir() -> const std::filesystem::path& {
        static const auto desktopDir = userHomeDir() / "Desktop";
        return desktopDir;
    }

    inline auto documentsDir() -> const std::filesystem::path& {
        static const auto documentsDir = userHomeDir() / "Documents";
        return documentsDir;
    }

    inline auto downloadsDir() -> const std::filesystem::path& {
        static const auto downloadsDir = userHomeDir() / "Downloads";
        return downloadsDir;
    }

    inline auto musicDir() -> const std::filesystem::path& {
        static const auto musicDir = userHomeDir() / "Music";
        return musicDir;
    }

    inline auto picturesDir() -> const std::filesystem::path& {
        static const auto picturesDir = userHomeDir() / "Pictures";
        return picturesDir;
    }

    inline auto videosDir() -> const std::filesystem::path& {
        static const auto videosDir = userHomeDir() / "Videos";
        return videosDir;
    }

    inline auto cacheDir() -> const std::filesystem::path& {
        #ifdef __linux__
        static const auto cacheDir = userHomeDir() / ".cache" / "blaadpapers";
        #elif _WIN32
        static const auto cacheDir = localAppDataDir() / "blaadpapers" / "cache";
        #endif

        return cacheDir;
    }

    inline auto configDir() -> const std::filesystem::path& {
        #ifdef __linux__
        static const auto configDir = userHomeDir() / ".config" / "blaadpapers";
        #elif _WIN32
        static const auto configDir = appDataDir() / "blaadpapers";
        #endif

        return configDir;
    }

    inline auto localDataDir() -> const std::filesystem::path& {
        #ifdef __linux__
        static const auto localDataDir = userHomeDir() / ".local" / "share" / "blaadpapers";
        #elif _WIN32
        static const auto localDataDir = localAppDataDir() / "blaadpapers";
        #endif

        return localDataDir;
    }

    inline auto stateDir() -> const std::filesystem::path& {
        #ifdef __linux__
        static const auto stateDir = userHomeDir() / ".local" / "state" / "blaadpapers";
        #elif _WIN32
        static const auto stateDir = localAppDataDir() / "blaadpapers" / "state";
        #endif

        return stateDir;
    }

    inline auto localDownloadsDir() -> const std::filesystem::path& {
        static const auto localDownloadsDir = localDataDir() / "downloads";
        return localDownloadsDir;
    }

    inline auto currentWallpaperIdFilePath() -> const std::filesystem::path& {
        static const auto currentWallpaperIdFilePath = stateDir() / "current_wallpaper";
        return currentWallpaperIdFilePath;
    }

    /**
     * Returns false on error. If directory already exists, or was successfully created, returns true.
     */
    inline auto createDirIfNotExists(const std::filesystem::path& path) -> bool {
        std::error_code errorCode;
        std::filesystem::create_directories(path, errorCode);
        return std::filesystem::is_directory(path, errorCode);
    }
}

#ifdef _WIN32
template<>
struct std::formatter<std::filesystem::path> : std::formatter<std::string> {
    auto format(const std::filesystem::path& path, std::format_context& ctx) const {
        return std::formatter<std::string>::format(
            path.string(),
            ctx
        );
    }
};
#endif
