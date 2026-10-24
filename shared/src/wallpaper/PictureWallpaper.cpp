// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper/PictureWallpaper.hpp"

#include <format>
#include "util/StringUtils.hpp"

PictureWallpaper::PictureWallpaper(
    std::filesystem::path filePath,
    std::string id,
    std::string name,
    std::string source,
    std::vector<std::string> tags,
    const Size resolution
) : Wallpaper(std::move(filePath), std::move(id), std::move(name), std::move(source), std::move(tags)), resolution(resolution) {}

auto PictureWallpaper::getResolution() const noexcept -> const Size& {
    return resolution;
}

auto PictureWallpaper::toString() const noexcept -> std::string {
    return std::format(
        "{}\n"
        "    Name: {}\n"
        "    FilePath: {}\n"
        "    Resolution: {}\n"
        "    Source: {}\n"
        "    Tags: {}\n"
        "    Type: {}\n",
        id,
        name,
        filePath.string(),
        resolution,
        source,
        util::join(tags),
        "Picture"
    );
}

auto PictureWallpaper::yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept -> yyjson_mut_val* {
    const auto wallpaperData = yyjson_mut_obj(doc);

    const auto tagsData = yyjson_mut_arr(doc);
    for(const auto& tag : tags) {
        yyjson_mut_arr_add_str(doc, tagsData, tag.c_str());
    }

    yyjson_mut_obj_add_str(doc, wallpaperData, "id", id.c_str());
    yyjson_mut_obj_add_str(doc, wallpaperData, "name", name.c_str());
    yyjson_mut_obj_add_strcpy(doc, wallpaperData, "file_path", filePath.string().c_str());
    yyjson_mut_obj_add_size(doc, wallpaperData, "resolution", &resolution);
    yyjson_mut_obj_add_str(doc, wallpaperData, "source", source.c_str());
    yyjson_mut_obj_add_val(doc, wallpaperData, "tags", tagsData);
    yyjson_mut_obj_add_strcpy(doc, wallpaperData, "type", "Picture");

    return wallpaperData;
}
