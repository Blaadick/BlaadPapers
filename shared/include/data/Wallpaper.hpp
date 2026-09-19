// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <yyjson.h>

class Wallpaper {
public:
    virtual ~Wallpaper() = default;

    virtual auto toString() const noexcept -> std::string = 0;

    virtual auto yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept -> yyjson_mut_val* = 0;

    auto getId() const noexcept -> const std::string&;

    auto getFilePath() const noexcept -> const std::filesystem::path&;

    auto getDirPath() const noexcept -> const std::filesystem::path&;

    auto getName() const noexcept -> const std::string&;

    auto getSource() const noexcept -> const std::string&;

    auto getTags() const noexcept -> const std::vector<std::string>&;

protected:
    std::string id;
    std::filesystem::path filePath;
    std::filesystem::path dirPath;
    std::string name;
    std::string source;
    std::vector<std::string> tags;
};
