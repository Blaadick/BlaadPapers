// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Wallpaper.hpp"

#include "config/Config.hpp"

const std::string& Wallpaper::getId() const {
    return id;
}

const std::filesystem::path& Wallpaper::getFilePath() const {
    return filePath;
}

const std::filesystem::path& Wallpaper::getDirPath() const {
    return dirPath;
}

const std::string& Wallpaper::getName() const {
    return name;
}

const Size& Wallpaper::getResolution() const {
    return resolution;
}

const std::string& Wallpaper::getSource() const {
    return source;
}

const std::vector<std::string>& Wallpaper::getTags() const {
    return tags;
}
