// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <expected>
#include <filesystem>
#include <yyjson.h>
#include "file_processing/json/JsonDocHolder.hpp"
#include "util/Pointers.hpp"

class JsonArr;

class JsonObj {
public:
    JsonObj(sptr<JsonDocHolder> doc, yyjson_val* root);

    [[nodiscard]]
    static std::expected<JsonObj, std::string> tryParse(const std::filesystem::path& filePath) noexcept;

    [[nodiscard]]
    static JsonObj parse(const std::filesystem::path& filePath);

    [[nodiscard]]
    std::expected<JsonObj, std::string> tryGetObj(std::string_view key) const noexcept;

    [[nodiscard]]
    JsonObj getObj(std::string_view key) const;

    [[nodiscard]]
    std::expected<JsonArr, std::string> tryGetArr(std::string_view key) const noexcept;

    [[nodiscard]]
    JsonArr getArr(std::string_view key) const;

    [[nodiscard]]
    std::expected<std::string_view, std::string> tryGetString(std::string_view key) const noexcept;

    [[nodiscard]]
    std::string_view getString(std::string_view key) const;

    [[nodiscard]]
    std::expected<bool, std::string> tryGetBool(std::string_view key) const noexcept;

    [[nodiscard]]
    bool getBool(std::string_view key) const;

private:
    sptr<JsonDocHolder> doc;
    yyjson_val* root;
};
