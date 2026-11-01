// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:PictureWallpaper;

import std;
import yyjson;
import :Size;
import :Wallpaper;

export class PictureWallpaper final : public Wallpaper {
public:
    PictureWallpaper(
        std::filesystem::path filePath,
        std::string id,
        std::string name,
        std::string source,
        std::vector<std::string> tags,
        Size resolution
    );

    auto getResolution() const noexcept -> const Size&;

    auto toString() const noexcept -> std::string override;

    auto yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept -> yyjson_mut_val* override;

private:
    Size resolution;
};
