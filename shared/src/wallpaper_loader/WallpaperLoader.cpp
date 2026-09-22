// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper_loader/WallpaperLoader.hpp"

#include <algorithm>
#include <string>
#include <unordered_set>

WallpaperLoader::WallpaperLoader(
    std::unordered_set<const file::FileType*> supportedFileTypes
) : supportedFileTypes(std::move(supportedFileTypes)) {}

auto WallpaperLoader::getSupportedFileTypes() const -> const std::unordered_set<const file::FileType*>& {
    return supportedFileTypes;
}

auto WallpaperLoader::isSupported(const std::filesystem::path& filePath) const -> bool {
    auto fileType = file::getTypeFromFile(filePath);
    if(!fileType.has_value()) {
        return false;
    }

    return supportedFileTypes.contains(&fileType.value());
}

auto WallpaperLoader::loadWallpaperData(const std::filesystem::path& wallpaperDataFilePath) const -> std::optional<WallpaperData> {
    if(!std::filesystem::exists(wallpaperDataFilePath)) {
        return std::nullopt;
    }

    WallpaperData wallpaperData;

    yyjson_read_err err;
    const auto doc = yyjson_read_file(wallpaperDataFilePath.c_str(), YYJSON_READ_NOFLAG, nullptr, &err);
    if(!doc) {
        return std::nullopt;
    }

    const auto root = yyjson_doc_get_root(doc);
    if(!yyjson_is_obj(root)) {
        return std::nullopt;
    }

    const auto wallpaperNameData = yyjson_obj_get(root, "name");
    if(yyjson_is_str(wallpaperNameData)) {
        wallpaperData.name = unsafe_yyjson_get_str(wallpaperNameData);
    }

    const auto wallpaperSourceData = yyjson_obj_get(root, "source");
    if(yyjson_is_str(wallpaperSourceData)) {
        wallpaperData.source = unsafe_yyjson_get_str(wallpaperSourceData);
    }

    const auto wallpaperTagsData = yyjson_obj_get(root, "tags");
    if(yyjson_is_arr(wallpaperTagsData)) {
        size_t i, max;
        yyjson_val* item;
        yyjson_arr_foreach(wallpaperTagsData, i, max, item) {
            if(yyjson_is_str(item)) {
                wallpaperData.tags.emplace_back(yyjson_get_str(item));
            }
        }
    }

    yyjson_doc_free(doc);
    return wallpaperData;
}

bool WallpaperLoader::saveWallpaperData(
    const std::filesystem::path& wallpaperDataFilePath,
    const WallpaperData& wallpaperData
) const {
    const auto doc = yyjson_mut_doc_new(nullptr);
    const auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    const auto wallpaperTagsData = yyjson_mut_arr(doc);
    for(const auto& tag : wallpaperData.tags) {
        yyjson_mut_arr_add_str(doc, wallpaperTagsData, tag.c_str());
    }

    yyjson_mut_obj_add_str(doc, root, "name", wallpaperData.name.c_str());
    yyjson_mut_obj_add_str(doc, root, "source", wallpaperData.source.c_str());
    yyjson_mut_obj_add_val(doc, root, "tags", wallpaperTagsData);

    yyjson_write_err writeErr;
    const auto isWritten = yyjson_mut_write_file(wallpaperDataFilePath.c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        return false;
    }

    yyjson_mut_doc_free(doc);
    return true;
}
