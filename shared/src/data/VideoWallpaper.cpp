// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/VideoWallpaper.hpp"

#include <format>

VideoWallpaper::VideoWallpaper(
    std::string id,
    std::filesystem::path filePath,
    std::filesystem::path dirPath,
    std::string name,
    const Size resolution,
    const int frameRate,
    std::string source,
    std::vector<std::string> tags
) {
    this->id = std::move(id);
    this->filePath = std::move(filePath);
    this->dirPath = std::move(dirPath);
    this->name = std::move(name);
    this->resolution = resolution;
    this->frameRate = frameRate;
    this->source = std::move(source);
    this->tags = std::move(tags);
}

int VideoWallpaper::getFrameRate() const {
    return frameRate;
}

nlohmann::json VideoWallpaper::toJson() const {
    return {
        {"id", id},
        {"file_path", filePath},
        {"name", name},
        {"resolution", resolution.toString()},
        {"frame_rate", frameRate},
        {"source", source},
        {"tags", tags},
        {"type", "Video"}
    };
}

std::string VideoWallpaper::toString() const {
    return std::format(
        "{}\n"
        "    Name: {}\n"
        "    FilePath: {}\n"
        "    Resolution: {}\n"
        "    FrameRate: {}\n"
        "    Source: {}\n"
        "    Tags: {}\n"
        "    Type: {}\n",
        id,
        name,
        filePath.string(),
        resolution.toString(),
        frameRate,
        source,
        tags,
        "Video"
    );
}
