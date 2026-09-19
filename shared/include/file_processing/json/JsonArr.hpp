// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <yyjson.h>
#include "file_processing/json/JsonDocHolder.hpp"
#include "file_processing/json/JsonObj.hpp"
#include "util/Pointers.hpp"

class JsonObj;

class JsonArr {
public:
    JsonArr(sptr<JsonDocHolder> doc, yyjson_val* root);

    static auto tryParse(const std::filesystem::path& filePath) noexcept -> std::expected<JsonArr, std::string>;

    static auto parse(const std::filesystem::path& filePath) -> JsonArr;

    template<std::invocable<const JsonObj&> F>
    void forEachObj(F&& function) const {
        size_t i, max;
        yyjson_val* item;
        yyjson_arr_foreach(root, i, max, item) {
            if(yyjson_is_obj(item)) {
                function(JsonObj(doc, item));
            }
        }
    }

    template<std::invocable<std::string_view> F>
    void forEachString(F&& function) const {
        size_t i, max;
        yyjson_val* item;
        yyjson_arr_foreach(root, i, max, item) {
            if(yyjson_is_str(item)) {
                function(unsafe_yyjson_get_str(item));
            }
        }
    }

private:
    sptr<JsonDocHolder> doc;
    yyjson_val* root;
};
