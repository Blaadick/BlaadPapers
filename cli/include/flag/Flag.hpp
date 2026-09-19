// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <string_view>
#include <yyjson.h>

struct Flag {
    std::string name;
    std::optional<char> shortName;
    std::string description;

    static auto isShortFlag(std::string_view string) -> bool;

    static auto isLongFlag(std::string_view string) -> bool;

    static auto isFlag(std::string_view string) -> bool;
};

auto yyjson_mut_flag(yyjson_mut_doc* doc, const Flag* flag) -> yyjson_mut_val*;

auto yyjson_mut_arr_add_flag(yyjson_mut_doc* doc, yyjson_mut_val* arr, const Flag* flag) -> bool;
