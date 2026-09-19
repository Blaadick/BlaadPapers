// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Wallpaper.hpp"

#include "config/Config.hpp"

const std::string& Wallpaper::getId() const noexcept {
    return id;
}

const std::filesystem::path& Wallpaper::getFilePath() const noexcept {
    return filePath;
}

const std::filesystem::path& Wallpaper::getDirPath() const noexcept {
    return dirPath;
}

const std::string& Wallpaper::getName() const noexcept {
    return name;
}

const std::string& Wallpaper::getSource() const noexcept {
    return source;
}

const std::vector<std::string>& Wallpaper::getTags() const noexcept {
    return tags;
}
