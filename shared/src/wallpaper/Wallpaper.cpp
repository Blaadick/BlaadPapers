// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper/Wallpaper.hpp"

auto Wallpaper::getFilePath() const noexcept -> const std::filesystem::path& {
    return filePath;
}

auto Wallpaper::getRootDirPath() const noexcept -> std::filesystem::path {
    return filePath.parent_path();
}

auto Wallpaper::getId() const noexcept -> const std::string& {
    return id;
}

auto Wallpaper::getName() const noexcept -> const std::string& {
    return name;
}

auto Wallpaper::getSource() const noexcept -> const std::string& {
    return source;
}

auto Wallpaper::getTags() const noexcept -> const std::vector<std::string>& {
    return tags;
}

Wallpaper::Wallpaper(
    std::filesystem::path filePath,
    std::string id,
    std::string name,
    std::string source,
    std::vector<std::string> tags
) : filePath(std::move(filePath)), id(std::move(id)), name(std::move(name)), source(std::move(source)), tags(std::move(tags)) {}
