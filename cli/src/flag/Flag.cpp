// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "flag/Flag.hpp"

bool Flag::isShortFlag(const std::string_view string) {
    return string.starts_with('-') && string.length() == 2;
}

bool Flag::isLongFlag(const std::string_view string) {
    return string.starts_with("--") && string.length() > 2;
}

bool Flag::isFlag(const std::string_view string) {
    return isShortFlag(string) || isLongFlag(string);
}

yyjson_mut_val* yyjson_mut_flag(yyjson_mut_doc* doc, const Flag* flag) {
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

bool yyjson_mut_arr_add_flag(yyjson_mut_doc* doc, yyjson_mut_val* arr, const Flag* flag) {
    const auto val = yyjson_mut_flag(doc, flag);
    return yyjson_mut_arr_append(arr, val);
}
