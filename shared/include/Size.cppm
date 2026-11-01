// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:Size;

import std;
import yyjson;

export struct Size {
    int width = -1;
    int height = -1;

    auto operator*(const int& number) const -> Size;
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

export auto operator<<(std::ostream& os, const Size& size) -> std::ostream&;

export auto yyjson_mut_size(yyjson_mut_doc* doc, const Size* size) noexcept -> yyjson_mut_val*;

export auto yyjson_mut_obj_add_size(yyjson_mut_doc* doc, yyjson_mut_val* obj, const char* key, const Size* size) noexcept -> bool;
