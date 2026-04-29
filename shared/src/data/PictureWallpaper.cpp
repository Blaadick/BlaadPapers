// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/PictureWallpaper.hpp"

PictureWallpaper::PictureWallpaper(
    std::string id,
    std::string filePath,
    std::string name,
    Size resolution,
    std::string source,
    std::vector<std::string> tags
) {
    this->id = std::move(id);
    this->filePath = std::move(filePath);
    this->name = std::move(name);
    this->resolution = std::move(resolution);
    this->source = std::move(source);
    this->tags = std::move(tags);
}

nlohmann::json PictureWallpaper::toJson() const {
    return {
        {"id", id},
        {"path", filePath},
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
        "    Id: {}\n"
        "    Path: {}\n"
        "    Resolution: {}\n"
        "    Source: {}\n"
        "    Tags: {}\n"
        "    Type: {}\n",
        name,
        id,
        filePath.native(),
        resolution.toString(),
        source,
        tags,
        "Picture"
    );
}
