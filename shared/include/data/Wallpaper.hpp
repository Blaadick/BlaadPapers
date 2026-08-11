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

    [[nodiscard]]
    const std::string& getId() const;

    [[nodiscard]]
    const std::filesystem::path& getFilePath() const;

    [[nodiscard]]
    const std::filesystem::path& getDirPath() const;

    [[nodiscard]]
    const std::string& getName() const;

    [[nodiscard]]
    const std::string& getSource() const;

    [[nodiscard]]
    const std::vector<std::string>& getTags() const;

    [[nodiscard]]
    virtual std::string toString() const = 0;

    [[nodiscard]]
    virtual yyjson_mut_val* yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const = 0;

protected:
    std::string id;
    std::filesystem::path filePath;
    std::filesystem::path dirPath;
    std::string name;
    std::string source;
    std::vector<std::string> tags;
};
