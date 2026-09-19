// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

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

    auto getResolution() const noexcept -> const Size&;

    auto getFrameRate() const noexcept -> int;

    auto toString() const noexcept -> std::string override;

    auto yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept -> yyjson_mut_val* override;

protected:
    Size resolution;
    int frameRate;
};
