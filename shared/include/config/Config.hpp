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

    void loadGeneral();

    void loadGui();

    void loadApi();

    void save() const;

    void saveGeneral() const;

    void saveGui() const;

    void saveApi() const;

    [[nodiscard]]
    const std::filesystem::path& getWallpapersDirPath() const;

    [[nodiscard]]
    const std::vector<std::string>& getBadTags() const;

    [[nodiscard]]
    bool isWallpaperBad(const Wallpaper& wallpaper) const;

    [[nodiscard]]
    const std::optional<std::string>& getWallhavenApiKey() const;

    [[nodiscard]]
    const std::optional<std::string>& getDanbooruLogin() const;

    [[nodiscard]]
    const std::optional<std::string>& getDanbooruApiKey() const;

    [[nodiscard]]
    bool getStatusBarVisible() const;

    void setStatusBarVisible(bool newVisibility);

    [[nodiscard]]
    const std::filesystem::path& generalConfigFilePath() const;

    [[nodiscard]]
    const std::filesystem::path& guiConfigFilePath() const;

    [[nodiscard]]
    const std::filesystem::path& apiConfigFilePath() const;

private:
    sptr<util::Logger> logger;

    std::filesystem::path wallpapersDirPath = util::documentsDir() / "Wallpapers";
    std::vector<std::string> badTags = {"Sensitive", "Questionable", "Explicit"};
    std::optional<std::string> wallhavenApiKey = std::nullopt;
    std::optional<std::string> danbooruLogin = std::nullopt;
    std::optional<std::string> danbooruApiKey = std::nullopt;
    bool isStatusBarVisible = false;
};
