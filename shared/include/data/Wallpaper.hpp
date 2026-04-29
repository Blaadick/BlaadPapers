// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "data/Size.hpp"

class Wallpaper {
public:
    virtual ~Wallpaper() = default;

    [[nodiscard]]
    const std::string& getId() const;

    [[nodiscard]]
    const std::filesystem::path& getFilePath() const;

    [[nodiscard]]
    const std::string& getName() const;

    [[nodiscard]]
    const Size& getResolution() const;

    [[nodiscard]]
    const std::string& getSource() const;

    [[nodiscard]]
    const std::vector<std::string>& getTags() const;

    [[nodiscard]]
    bool isBad() const;

    bool apply() const;

    [[nodiscard]]
    virtual nlohmann::json toJson() const = 0;

    [[nodiscard]]
    virtual std::string toString() const = 0;

    [[nodiscard]]
    bool operator==(const Wallpaper& other) const;

protected:
    std::string id;
    std::filesystem::path filePath;
    std::string name;
    Size resolution;
    std::string source;
    std::vector<std::string> tags;
};
