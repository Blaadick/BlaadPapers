// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "data/Wallpaper.hpp"
#include "logger/Logger.hpp"
#include "util/Pointers.hpp"

class Config {
public:
    explicit Config(sptr<util::Logger> logger);

    void load();

    [[nodiscard]]
    std::vector<std::string> getBadTags();

    [[nodiscard]]
    std::filesystem::path getWallpapersDirPath();

    [[nodiscard]]
    bool getStatusBarVisible() const;

    void setStatusBarVisible(bool newVisibility);

    bool isWallpaperBad(const Wallpaper& wallpaper);

private:
    sptr<util::Logger> logger;

    std::vector<std::string> badTags;
    std::filesystem::path wallpapersDirPath;
    bool isStatusBarVisible = false;

    template<typename T>
    void updateConfig(const std::string& settingName, const T& value);
};
