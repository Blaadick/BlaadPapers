// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/PictureWallpaper.hpp"

#include <format>

PictureWallpaper::PictureWallpaper(
    std::string id,
    std::filesystem::path filePath,
    std::filesystem::path dirPath,
    std::string name,
    const Size resolution,
    std::string source,
    std::vector<std::string> tags
) {
    this->id = std::move(id);
    this->filePath = std::move(filePath);
    this->dirPath = std::move(dirPath);
    this->name = std::move(name);
    this->resolution = resolution;
    this->source = std::move(source);
    this->tags = std::move(tags);
}

std::string PictureWallpaper::toString() const {
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
        resolution.toString(),
        source,
        tags,
        "Picture"
    );
}

yyjson_mut_val* PictureWallpaper::yyjson_mut_wallpaper_obj(yyjson_mut_doc* doc) const {
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
