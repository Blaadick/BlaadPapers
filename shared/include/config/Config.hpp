// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <vector>
#include "SecretStorage.hpp"
#include "logger/Logger.hpp"
#include "util/PathUtils.hpp"
#include "util/Pointers.hpp"
#include "wallpaper/Wallpaper.hpp"

class Config final {
public:
    Config(sptr<util::Logger> logger, sptr<SecretStorage> secretStorage);

    void load();

    void loadGeneral();

    void loadGui();

    void save() const;

    void saveGeneral() const;

    void saveGui() const;

    auto getWallpapersFolder() const noexcept -> const std::filesystem::path&;

    void setWallpapersFolder(std::filesystem::path newPath) noexcept;

    auto getDefaultTags() const noexcept -> const std::vector<std::string>&;

    void setDefaultTags(std::vector<std::string> newTags) noexcept;

    auto getBadTags() const noexcept -> const std::vector<std::string>&;

    void setBadTags(std::vector<std::string> newTags) noexcept;

    auto getDeleteWallpapersPermanently() const noexcept -> bool;

    void setDeleteWallpapersPermanently(bool newDeletePermanently) noexcept;

    auto isWallpaperBad(const Wallpaper& wallpaper) const noexcept -> bool;

    auto getPostInstallScript() const noexcept -> const std::string&;

    void setPostInstallScript(std::string newScript) noexcept;

    auto getPostApplyScript() const noexcept -> const std::string&;

    void setPostApplyScript(std::string newScript) noexcept;

    auto getPostDeleteScript() const noexcept -> const std::string&;

    void setPostDeleteScript(std::string newScript) noexcept;

    auto getWallhavenApiKey() const -> std::expected<std::string, std::string>;

    void setWallhavenApiKey(const std::string& newKey) const;

    auto getDanbooruLogin() const -> std::expected<std::string, std::string>;

    void setDanbooruLogin(const std::string& newLogin) const;

    auto getDanbooruApiKey() const -> std::expected<std::string, std::string>;

    void setDanbooruApiKey(const std::string& newKey) const;

    auto getStatusBarVisible() const noexcept -> bool;

    void setStatusBarVisible(bool newVisibility) noexcept;

    auto getBadTaggedWallpapersVisible() const noexcept -> bool;

    void setBadTaggedWallpapersVisible(bool newVisibility) noexcept;

    auto getNativeTextRendering() const noexcept -> bool;

    void setNativeTextRendering(bool newNativeTextRendering) noexcept;

    auto getColorScheme() const noexcept -> const std::string&;

    void setColorScheme(std::string_view newColorScheme) noexcept;

    auto getIconTheme() const noexcept -> const std::string&;

    void setIconTheme(std::string_view newIconTheme) noexcept;

    auto getLanguage() const noexcept -> const std::string&;

    void setLanguage(std::string_view newLanguage) noexcept;

    auto generalConfigFilePath() const noexcept -> const std::filesystem::path&;

    auto guiConfigFilePath() const noexcept -> const std::filesystem::path&;

private:
    sptr<util::Logger> logger;
    sptr<SecretStorage> secretStorage;

    std::filesystem::path wallpapersFolder = util::documentsDir() / "Wallpapers";
    std::vector<std::string> defaultTags = {"General"};
    std::vector<std::string> badTags = {"Sensitive", "Questionable", "Explicit"};
    bool isDeleteWallpapersPermanently = false;
    std::string postInstallScript;
    std::string postApplyScript;
    std::string postDeleteScript;
    bool isBadTaggedWallpapersVisible = true;
    bool isStatusBarVisible = false;
    bool isNativeTextRendering = false;
    std::string colorScheme = "system";
    std::string iconTheme = "material";
    std::string language = "system";
};
