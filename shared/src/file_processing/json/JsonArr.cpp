// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "file_processing/json/JsonArr.hpp"

#include "file_processing/json/JsonError.hpp"

JsonArr::JsonArr(sptr<JsonDocHolder> doc, yyjson_val* root) : doc(std::move(doc)), root(root) {}

auto JsonArr::tryParse(const std::filesystem::path& filePath) noexcept -> std::expected<JsonArr, std::string> {
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

auto JsonArr::parse(const std::filesystem::path& filePath) -> JsonArr {
    auto jsonArr = tryParse(filePath);
    if(!jsonArr.has_value()) {
        throw JsonError(jsonArr.error());
    }

    return *jsonArr;
}
