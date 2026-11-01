// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:VideoWallpaper;

import std;
import yyjson;
import :Size;
import :Wallpaper;

export class VideoWallpaper final : public Wallpaper {
public:
    VideoWallpaper(
        std::filesystem::path filePath,
        std::string id,
        std::string name,
        std::string source,
        std::vector<std::string> tags,
        Size resolution,
        int frameRate
    );

    auto getResolution() const noexcept -> const Size&;

    auto getFrameRate() const noexcept -> int;

    auto toString() const noexcept -> std::string override;

    auto yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept -> yyjson_mut_val* override;

private:
    Size resolution;
    int frameRate;
};
