// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/PictureWallpaper.hpp"

const std::unordered_set<std::string> PictureWallpaper::supportedFormats = {
    ".png",
    ".apng",
    ".jpeg",
    ".webp",
    ".avif",
    ".tiff",
    ".heic"
};

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

nlohmann::json PictureWallpaper::toJson() const {
    return {
        {"id", id},
        {"file_path", filePath},
        {"name", name},
        {"resolution", resolution.toString()},
        {"source", source},
        {"tags", tags},
        {"type", "Picture"}
    };
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
        filePath.native(),
        resolution.toString(),
        source,
        tags,
        "Picture"
    );
}
