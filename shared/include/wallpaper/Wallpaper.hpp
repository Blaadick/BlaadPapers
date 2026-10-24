// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

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

    auto getFilePath() const noexcept -> const std::filesystem::path&;

    auto getRootDirPath() const noexcept -> std::filesystem::path;

    auto getId() const noexcept -> const std::string&;

    auto getName() const noexcept -> const std::string&;

    auto getSource() const noexcept -> const std::string&;

    auto getTags() const noexcept -> const std::vector<std::string>&;

protected:
    std::filesystem::path filePath;
    std::string id;
    std::string name;
    std::string source;
    std::vector<std::string> tags;

    Wallpaper(
        std::filesystem::path filePath,
        std::string id,
        std::string name,
        std::string source,
        std::vector<std::string> tags
    );
};
