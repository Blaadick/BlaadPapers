// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "flag/Flag.hpp"

auto Flag::isShortFlag(const std::string_view string) -> bool {
    return string.starts_with('-') && string.length() == 2;
}

auto Flag::isLongFlag(const std::string_view string) -> bool {
    return string.starts_with("--") && string.length() > 2;
}

auto Flag::isFlag(const std::string_view string) -> bool {
    return isShortFlag(string) || isLongFlag(string);
}

auto yyjson_mut_flag(yyjson_mut_doc* doc, const Flag* flag) -> yyjson_mut_val* {
    const auto flagData = yyjson_mut_obj(doc);

    yyjson_mut_obj_add_str(doc, flagData, "name", flag->name.c_str());

    if(flag->shortName.has_value()) {
        const char buf[2] = {flag->shortName.value(), '\0'};
        yyjson_mut_obj_add_strcpy(doc, flagData, "short_name", buf);
    } else {
        yyjson_mut_obj_add_null(doc, flagData, "short_name");
    }

    yyjson_mut_obj_add_str(doc, flagData, "description", flag->description.c_str());

    return flagData;
}

auto yyjson_mut_arr_add_flag(yyjson_mut_doc* doc, yyjson_mut_val* arr, const Flag* flag) -> bool {
    const auto val = yyjson_mut_flag(doc, flag);
    return yyjson_mut_arr_append(arr, val);
}
