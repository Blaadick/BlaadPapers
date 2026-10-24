// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wallpaper/loader/WallpaperLoader.hpp"

#include <algorithm>
#include <string>
#include <unordered_set>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"

WallpaperLoader::WallpaperLoader(
    sptr<Config> config,
    std::unordered_set<const file::FileType*> supportedFileTypes
) : config(std::move(config)), supportedFileTypes(std::move(supportedFileTypes)) {}

auto WallpaperLoader::getSupportedFileTypes() const noexcept -> const std::unordered_set<const file::FileType*>& {
    return supportedFileTypes;
}

auto WallpaperLoader::isSupported(const std::filesystem::path& filePath) const -> bool {
    auto fileType = file::getTypeFromFile(filePath);
    if(!fileType.has_value()) {
        return false;
    }

    return supportedFileTypes.contains(&fileType.value());
}

auto WallpaperLoader::getDefaultWallpaperData(std::string wallpaperId) const noexcept -> WallpaperData {
    return WallpaperData(std::move(wallpaperId), "", config->getDefaultTags());
}

auto WallpaperLoader::loadWallpaperData(const std::filesystem::path& wallpaperDataFilePath) const -> std::optional<WallpaperData> {
    if(!std::filesystem::exists(wallpaperDataFilePath)) {
        auto defaultWallpaperData = getDefaultWallpaperData(wallpaperDataFilePath.parent_path().stem().string());
        if(!saveWallpaperData(wallpaperDataFilePath, defaultWallpaperData)) {
            return std::nullopt;
        }

        return defaultWallpaperData;
    }

    try {
        WallpaperData wallpaperData;

        auto jsonObj = JsonObj::parse(wallpaperDataFilePath);
        wallpaperData.name = jsonObj.getString("name");
        wallpaperData.source = jsonObj.getString("source");

        auto wallpaperTagsVal = jsonObj.getArr("tags");
        wallpaperTagsVal.forEachString(
            [&wallpaperData](const std::string_view& string) {
                wallpaperData.tags.emplace_back(string);
            }
        );

        return wallpaperData;
    } catch(const std::exception&) {
        return std::nullopt;
    }
}

auto WallpaperLoader::saveWallpaperData(
    const std::filesystem::path& wallpaperDataFilePath,
    const WallpaperData& wallpaperData
) const -> bool {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    auto wallpaperTagsData = yyjson_mut_arr(doc);
    for(auto& tag : wallpaperData.tags) {
        yyjson_mut_arr_add_str(doc, wallpaperTagsData, tag.c_str());
    }

    yyjson_mut_obj_add_str(doc, root, "name", wallpaperData.name.c_str());
    yyjson_mut_obj_add_str(doc, root, "source", wallpaperData.source.c_str());
    yyjson_mut_obj_add_val(doc, root, "tags", wallpaperTagsData);

    yyjson_write_err writeErr;
    auto isWritten = yyjson_mut_write_file(wallpaperDataFilePath.string().c_str(), doc, YYJSON_WRITE_PRETTY, nullptr, &writeErr);
    if(!isWritten) {
        yyjson_mut_doc_free(doc);
        return false;
    }

    yyjson_mut_doc_free(doc);
    return true;
}
