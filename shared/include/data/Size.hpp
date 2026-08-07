// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <format>
#include <yyjson.h>

struct Size {
    int width = -1;
    int height = -1;

    Size operator*(const int& numberww) const;
};

template<>
struct std::formatter<Size> : std::formatter<std::string> {
    auto format(const Size& size, std::format_context& ctx) const {
        return std::formatter<std::string>::format(
            std::format("{}x{}", size.width, size.height),
            ctx
        );
    }
};

std::ostream& operator<<(std::ostream& os, const Size& size);

yyjson_mut_val* yyjson_mut_size(yyjson_mut_doc* doc, const Size* size);

bool yyjson_mut_obj_add_size(yyjson_mut_doc* doc, yyjson_mut_val* obj, const char* key, const Size* size);
