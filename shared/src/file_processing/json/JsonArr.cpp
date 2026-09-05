// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "file_processing/json/JsonArr.hpp"

#include "file_processing/json/JsonError.hpp"
#include "file_processing/json/JsonObj.hpp"

JsonArr::JsonArr(sptr<JsonDocHolder> doc, yyjson_val* root) : doc(std::move(doc)), root(root) {}

std::expected<JsonArr, std::string> JsonArr::tryParse(const std::filesystem::path& filePath) noexcept {
    yyjson_read_err readErr;
    auto doc = yyjson_read_file(filePath.c_str(), YYJSON_READ_NOFLAG, nullptr, &readErr);
    if(!doc) {
        return std::unexpected(readErr.msg);
    }

    auto root = yyjson_doc_get_root(doc);
    if(!yyjson_is_arr(root)) {
        yyjson_doc_free(doc);
        return std::unexpected("Root is not array");
    }

    return JsonArr(std::make_shared<JsonDocHolder>(doc), root);
}

JsonArr JsonArr::parse(const std::filesystem::path& filePath) {
    auto jsonArr = tryParse(filePath);
    if(!jsonArr.has_value()) {
        throw JsonError(jsonArr.error());
    }

    return *jsonArr;
}

void JsonArr::forEachObj(const std::function<void(const JsonObj&)>& function) const {
    size_t i, max;
    yyjson_val* item;
    yyjson_arr_foreach(root, i, max, item) {
        if(yyjson_is_obj(item)) {
            function(JsonObj(doc, item));
        }
    }
}

void JsonArr::forEachString(const std::function<void(std::string_view)>& function) const {
    size_t i, max;
    yyjson_val* item;
    yyjson_arr_foreach(root, i, max, item) {
        if(yyjson_is_str(item)) {
            function(unsafe_yyjson_get_str(item));
        }
    }
}
