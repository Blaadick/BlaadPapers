// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper/VideoWallpaper.hpp"

#include <format>
#include "util/StringUtils.hpp"

VideoWallpaper::VideoWallpaper(
    std::filesystem::path filePath,
    std::string id,
    std::string name,
    std::string source,
    std::vector<std::string> tags,
    const Size resolution,
    const int frameRate
) : Wallpaper(std::move(filePath), std::move(id), std::move(name), std::move(source), std::move(tags)), resolution(resolution), frameRate(frameRate) {}

auto VideoWallpaper::getResolution() const noexcept -> const Size& {
    return resolution;
}

auto VideoWallpaper::getFrameRate() const noexcept -> int {
    return frameRate;
}

auto VideoWallpaper::toString() const noexcept -> std::string {
    return std::format(
        "{}\n"
        "    Name: {}\n"
        "    FilePath: {}\n"
        "    Resolution: {}\n"
        "    FrameRate: {}\n"
        "    Source: {}\n"
        "    Tags: {}\n"
        "    Type: {}\n",
        id,
        name,
        filePath.string(),
        resolution,
        frameRate,
        source,
        util::join(tags),
        "Video"
    );
}

auto VideoWallpaper::yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept -> yyjson_mut_val* {
    const auto wallpaperData = yyjson_mut_obj(doc);

    const auto tagsData = yyjson_mut_arr(doc);
    for(const auto& tag : tags) {
        yyjson_mut_arr_add_str(doc, tagsData, tag.c_str());
    }

    yyjson_mut_obj_add_str(doc, wallpaperData, "id", id.c_str());
    yyjson_mut_obj_add_str(doc, wallpaperData, "name", name.c_str());
    yyjson_mut_obj_add_strcpy(doc, wallpaperData, "file_path", filePath.string().c_str());
    yyjson_mut_obj_add_size(doc, wallpaperData, "resolution", &resolution);
    yyjson_mut_obj_add_int(doc, wallpaperData, "frame_rate", frameRate);
    yyjson_mut_obj_add_str(doc, wallpaperData, "source", source.c_str());
    yyjson_mut_obj_add_val(doc, wallpaperData, "tags", tagsData);
    yyjson_mut_obj_add_strcpy(doc, wallpaperData, "type", "Video");

    return wallpaperData;
}
