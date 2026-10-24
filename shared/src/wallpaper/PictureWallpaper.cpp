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
) {
    this->filePath = std::move(filePath);
    this->id = std::move(id);
    this->name = std::move(name);
    this->source = std::move(source);
    this->tags = std::move(tags);
    this->resolution = resolution;
}

const Size& PictureWallpaper::getResolution() const noexcept {
    return resolution;
}

std::string PictureWallpaper::toString() const noexcept {
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

yyjson_mut_val* PictureWallpaper::yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const noexcept {
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
