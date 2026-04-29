// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Wallpaper.hpp"

class PictureWallpaper : public Wallpaper {
public:
    PictureWallpaper(
        std::string id,
        std::string filePath,
        std::string name,
        Size resolution,
        std::string source,
        std::vector<std::string> tags
    );

    [[nodiscard]]
    nlohmann::json toJson() const override;

    [[nodiscard]]
    std::string toString() const override;
};
