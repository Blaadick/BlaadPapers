// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:JsonObj;

import std;
import yyjson;
import :JsonDocHolder;
import :Pointers;

export class JsonArr;

export class JsonObj final {
public:
    JsonObj(sptr<JsonDocHolder> doc, yyjson_val* root);

    static auto tryParse(const std::filesystem::path& filePath) noexcept -> std::expected<JsonObj, std::string>;

    static auto parse(const std::filesystem::path& filePath) -> JsonObj;

    auto tryGetObj(std::string_view key) const noexcept -> std::expected<JsonObj, std::string>;

    auto getObj(std::string_view key) const -> JsonObj;

    auto tryGetArr(std::string_view key) const noexcept -> std::expected<JsonArr, std::string>;

    auto getArr(std::string_view key) const -> JsonArr;

    auto tryGetString(std::string_view key) const noexcept -> std::expected<std::string_view, std::string>;

    auto getString(std::string_view key) const -> std::string_view;

    auto tryGetBool(std::string_view key) const noexcept -> std::expected<bool, std::string>;

    auto getBool(std::string_view key) const -> bool;

    auto tryGetInt(std::string_view key) const noexcept -> std::expected<int, std::string>;

    auto getInt(std::string_view key) const -> int;

    auto tryGetDouble(std::string_view key) const noexcept -> std::expected<double, std::string>;

    auto getDouble(std::string_view key) const -> double;

private:
    sptr<JsonDocHolder> doc;
    yyjson_val* root;
};
