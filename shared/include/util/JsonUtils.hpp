// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <yyjson.h>
#include "data/Size.hpp"
#include "data/Wallpaper.hpp"

inline bool yyjson_mut_obj_add_size(yyjson_mut_doc* doc, yyjson_mut_val* obj, const char* key, const Size* val) {
    if(!val) {
        return false;
    }

    // TODO Use std::to_chars()
    const auto sizeData = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_str(doc, sizeData, "width", std::to_string(val->width).c_str());
    yyjson_mut_obj_add_str(doc, sizeData, "height", std::to_string(val->height).c_str());

    if(yyjson_mut_obj_add_val(doc, obj, key, sizeData)) {
        return false;
    }

    return true;
}

inline bool yyjson_mut_obj_add_wallpaper(yyjson_mut_doc* doc, yyjson_mut_val* obj, const char* key, const Wallpaper* val) {
    if(!val) {
        return false;
    }

    const auto wallpaperData = yyjson_mut_obj(doc);

    const auto tagsData = yyjson_mut_arr(doc);
    for(const auto& tag : val->getTags()) {
        yyjson_mut_arr_add_str(doc, tagsData, tag.c_str());
    }

    yyjson_mut_obj_add_str(doc, wallpaperData, "id", val->getId().c_str());
    yyjson_mut_obj_add_str(doc, wallpaperData, "file_path", val->getFilePath().c_str());
    yyjson_mut_obj_add_str(doc, wallpaperData, "name", val->getName().c_str());
    yyjson_mut_obj_add_size(doc, wallpaperData, "resolution", &val->getResolution());
    yyjson_mut_obj_add_str(doc, wallpaperData, "source", val->getSource().c_str());
    yyjson_mut_obj_add_val(doc, wallpaperData, "tags", tagsData);
    yyjson_mut_obj_add_strcpy(doc, wallpaperData, "type", "[TYPE]");

    if(yyjson_mut_obj_add_val(doc, obj, key, wallpaperData)) {
        return false;
    }

    return true;
}
