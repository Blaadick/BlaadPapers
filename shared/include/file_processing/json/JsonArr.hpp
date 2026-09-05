// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <expected>
#include <filesystem>
#include <functional>
#include <string>
#include <yyjson.h>
#include "file_processing/json/JsonDocHolder.hpp"
#include "util/Pointers.hpp"

class JsonObj;

class JsonArr {
public:
    JsonArr(sptr<JsonDocHolder> doc, yyjson_val* root);

    [[nodiscard]]
    static std::expected<JsonArr, std::string> tryParse(const std::filesystem::path& filePath) noexcept;

    [[nodiscard]]
    static JsonArr parse(const std::filesystem::path& filePath);

    void forEachObj(const std::function<void(const JsonObj&)>& function) const;

    void forEachString(const std::function<void(std::string_view)>& function) const;

private:
    sptr<JsonDocHolder> doc;
    yyjson_val* root;
};
