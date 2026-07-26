// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string_view>
#include <yyjson.h>

struct Flag {
    std::string name;
    std::optional<char> shortName;
    std::string description;

    [[nodiscard]]
    static bool isShortFlag(std::string_view string);

    [[nodiscard]]
    static bool isLongFlag(std::string_view string);

    [[nodiscard]]
    static bool isFlag(std::string_view string);
};

yyjson_mut_val* yyjson_mut_flag(yyjson_mut_doc* doc, const Flag* flag);

bool yyjson_mut_arr_add_flag(yyjson_mut_doc* doc, yyjson_mut_val* arr, const Flag* flag);
