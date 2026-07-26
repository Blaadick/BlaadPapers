// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/PictureWallpaper.hpp"

#include <format>

PictureWallpaper::PictureWallpaper(
    std::string id,
    std::filesystem::path filePath,
    std::filesystem::path dirPath,
    std::string name,
    const Size resolution,
    std::string source,
    std::vector<std::string> tags
) {
    this->id = std::move(id);
    this->filePath = std::move(filePath);
    this->dirPath = std::move(dirPath);
    this->name = std::move(name);
    this->resolution = resolution;
    this->source = std::move(source);
    this->tags = std::move(tags);
}

std::string PictureWallpaper::toString() const {
    return std::format(
        "{}\n"
        "    Name: {}\n"
        "    FilePath: {}\n"
        "    Resolution: {}\n"
        "    Source: {}\n"
        "    Tags: {}\n"
        "    Type: {}\n",
        id,
        name,
        filePath.string(),
        resolution.toString(),
        source,
        tags,
        "Picture"
    );
}
