// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <bits/fs_path.h>

class Config {
public:
    static void load();

    [[nodiscard]]
    static std::vector<std::string> getBadTags();

    [[nodiscard]]
    static std::vector<std::filesystem::path> getWallpaperDirPaths();

    [[nodiscard]]
    static bool getStatusBarVisible();

    static void setStatusBarVisible(bool newVisibility);

    static std::filesystem::path configPath();

private:
    static std::vector<std::string> badTags;
    static std::vector<std::filesystem::path> wallpaperDirPaths;
    static bool isStatusBarVisible;
};
