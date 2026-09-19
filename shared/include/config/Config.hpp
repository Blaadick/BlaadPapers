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

    auto getWallpapersDirPath() const noexcept -> const std::filesystem::path&;

    auto getBadTags() const noexcept -> const std::vector<std::string>&;

    auto isWallpaperBad(const Wallpaper& wallpaper) const noexcept -> bool;

    auto getWallhavenApiKey() const noexcept -> const std::optional<std::string>&;

    auto getDanbooruLogin() const noexcept -> const std::optional<std::string>&;

    auto getDanbooruApiKey() const noexcept -> const std::optional<std::string>&;

    auto getStatusBarVisible() const noexcept -> bool;

    void setStatusBarVisible(bool newVisibility) noexcept;

    auto generalConfigFilePath() const noexcept -> const std::filesystem::path&;

    auto guiConfigFilePath() const noexcept -> const std::filesystem::path&;

    auto apiConfigFilePath() const noexcept -> const std::filesystem::path&;

private:
    sptr<util::Logger> logger;

    std::filesystem::path wallpapersDirPath = util::documentsDir() / "Wallpapers";
    std::vector<std::string> badTags = {"Sensitive", "Questionable", "Explicit"};
    std::optional<std::string> wallhavenApiKey = std::nullopt;
    std::optional<std::string> danbooruLogin = std::nullopt;
    std::optional<std::string> danbooruApiKey = std::nullopt;
    bool isStatusBarVisible = false;
};
