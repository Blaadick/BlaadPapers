// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Wallpaper.hpp"
#include "data/Size.hpp"

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
    const Size& getResolution() const;

    [[nodiscard]]
    int getFrameRate() const;

    [[nodiscard]]
    std::string toString() const override;

    [[nodiscard]]
    yyjson_mut_val* yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const override;

protected:
    Size resolution;
    int frameRate;
};
