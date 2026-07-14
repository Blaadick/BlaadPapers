// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_set>
#include "Wallpaper.hpp"

class VideoWallpaper : public Wallpaper {
public:
    VideoWallpaper(
        std::string id,
        std::filesystem::path filePath,
        std::filesystem::path dirPath,
        std::string name,
        Size resolution,
        int frameRate,
        std::string source,
        std::vector<std::string> tags
    );

    [[nodiscard]]
    int getFrameRate() const;

    [[nodiscard]]
    nlohmann::json toJson() const override;

    [[nodiscard]]
    std::string toString() const override;

protected:
    int frameRate;
};
