// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Wallpaper.hpp"
#include "data/Size.hpp"

class PictureWallpaper : public Wallpaper {
public:
    PictureWallpaper(
        std::string id,
        std::filesystem::path filePath,
        std::filesystem::path dirPath,
        std::string name,
        Size resolution,
        std::string source,
        std::vector<std::string> tags
    );

    auto getResolution() noexcept -> const Size&;

    auto toString() const noexcept -> std::string override;

    auto yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept -> yyjson_mut_val* override;

private:
    Size resolution;
};
