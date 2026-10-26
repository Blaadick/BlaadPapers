// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "logger/Logger.hpp"
#include "util/PathUtils.hpp"
#include "util/Pointers.hpp"
#include "wallpaper/Wallpaper.hpp"

class Config final {
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

    void setWallpapersDirPath(std::filesystem::path newPath) noexcept;

    auto getDefaultTags() const noexcept -> const std::vector<std::string>&;

    void setDefaultTags(std::vector<std::string> newTags) noexcept;

    auto getBadTags() const noexcept -> const std::vector<std::string>&;

    void setBadTags(std::vector<std::string> newTags) noexcept;

    auto isWallpaperBad(const Wallpaper& wallpaper) const noexcept -> bool;

    auto getPostInstallScript() const noexcept -> const std::string&;

    void setPostInstallScript(std::string newScript) noexcept;

    auto getPostApplyScript() const noexcept -> const std::string&;

    void setPostApplyScript(std::string newScript) noexcept;

    auto getPostDeleteScript() const noexcept -> const std::string&;

    void setPostDeleteScript(std::string newScript) noexcept;

    auto getWallhavenApiKey() const noexcept -> const std::optional<std::string>&;

    void setWallhavenApiKey(std::optional<std::string> newKey) noexcept;

    auto getDanbooruLogin() const noexcept -> const std::optional<std::string>&;

    void setDanbooruLogin(std::optional<std::string> newLogin) noexcept;

    auto getDanbooruApiKey() const noexcept -> const std::optional<std::string>&;

    void setDanbooruApiKey(std::optional<std::string> newKey) noexcept;

    auto getStatusBarVisible() const noexcept -> bool;

    void setStatusBarVisible(bool newVisibility) noexcept;

    auto getBadTaggedWallpapersVisible() const noexcept -> bool;

    void setBadTaggedWallpapersVisible(bool newVisibility) noexcept;

    auto getNativeTextRendering() const noexcept -> bool;

    void setNativeTextRendering(bool newNativeTextRendering) noexcept;

    auto getColorScheme() const noexcept -> const std::string&;

    void setColorScheme(std::string_view newColorScheme) noexcept;

    auto generalConfigFilePath() const noexcept -> const std::filesystem::path&;

    auto guiConfigFilePath() const noexcept -> const std::filesystem::path&;

    auto apiConfigFilePath() const noexcept -> const std::filesystem::path&;

private:
    sptr<util::Logger> logger;

    std::filesystem::path wallpapersDirPath = util::documentsDir() / "Wallpapers";
    std::vector<std::string> defaultTags = {"General"};
    std::vector<std::string> badTags = {"Sensitive", "Questionable", "Explicit"};
    std::string postInstallScript;
    std::string postApplyScript;
    std::string postDeleteScript;
    std::optional<std::string> wallhavenApiKey = std::nullopt;
    std::optional<std::string> danbooruLogin = std::nullopt;
    std::optional<std::string> danbooruApiKey = std::nullopt;
    bool isBadTaggedWallpapersVisible = true;
    bool isStatusBarVisible = false;
    bool isNativeTextRendering = false;
    std::string colorScheme = "system";
};
