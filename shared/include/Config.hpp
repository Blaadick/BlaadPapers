// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "data/Wallpaper.hpp"
#include "logger/Logger.hpp"
#include "util/PathUtils.hpp"
#include "util/Pointers.hpp"

class Config {
public:
    explicit Config(sptr<util::Logger> logger);

    void load();

    [[nodiscard]]
    std::vector<std::string> getBadTags() const;

    [[nodiscard]]
    std::filesystem::path getWallpapersDirPath() const;

    [[nodiscard]]
    bool getStatusBarVisible() const;

    void setStatusBarVisible(bool newVisibility);

    [[nodiscard]]
    bool isWallpaperBad(const Wallpaper& wallpaper) const;

private:
    sptr<util::Logger> logger;

    std::vector<std::string> badTags = {"Sensitive", "Questionable", "Explicit"};
    std::filesystem::path wallpapersDirPath = util::documentsDirPath().append("Wallpapers");
    bool isStatusBarVisible = false;

    void saveConfig() const;
};
