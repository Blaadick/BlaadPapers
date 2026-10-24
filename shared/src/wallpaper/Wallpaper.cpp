// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper/Wallpaper.hpp"

const std::filesystem::path& Wallpaper::getFilePath() const noexcept {
    return filePath;
}

std::filesystem::path Wallpaper::getRootDirPath() const noexcept {
    return filePath.parent_path();
}

const std::string& Wallpaper::getId() const noexcept {
    return id;
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
