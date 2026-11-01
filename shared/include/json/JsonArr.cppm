// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:JsonArr;

import std;
import yyjson;
import :JsonDocHolder;
import :JsonObj;
import :Pointers;

export class JsonArr final {
public:
    JsonArr(sptr<JsonDocHolder> doc, yyjson_val* root);

    static auto tryParse(const std::filesystem::path& filePath) noexcept -> std::expected<JsonArr, std::string>;

    static auto parse(const std::filesystem::path& filePath) -> JsonArr;

    template<std::invocable<const JsonObj&> F>
    void forEachObj(F&& function) const {
        auto iter = yyjson_arr_iter_with(root);
        while(auto item = yyjson_arr_iter_next(&iter)) {
            if(yyjson_is_obj(item)) {
                function(JsonObj(doc, item));
            }
        }
    }

    template<std::invocable<std::string_view> F>
    void forEachString(F&& function) const {
        auto iter = yyjson_arr_iter_with(root);
        while(auto item = yyjson_arr_iter_next(&iter)) {
            if(yyjson_is_str(item)) {
                function(unsafe_yyjson_get_str(item));
            }
        }
    }

private:
    sptr<JsonDocHolder> doc;
    yyjson_val* root;
};
