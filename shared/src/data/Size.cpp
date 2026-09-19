// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Size.hpp"

auto Size::operator*(const int& number) const -> Size {
    return Size(width * number, height * number);
}

auto operator<<(std::ostream& os, const Size& size) -> std::ostream& {
    return os << std::format("{}x{}", size.width, size.height);
}

auto yyjson_mut_size(yyjson_mut_doc* doc, const Size* size) noexcept -> yyjson_mut_val* {
    const auto sizeData = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_int(doc, sizeData, "width", size->width);
    yyjson_mut_obj_add_int(doc, sizeData, "height", size->height);

    return sizeData;
}

auto yyjson_mut_obj_add_size(yyjson_mut_doc* doc, yyjson_mut_val* obj, const char* key, const Size* size) noexcept -> bool {
    const auto val = yyjson_mut_size(doc, size);
    return yyjson_mut_obj_add_val(doc, obj, key, val);
}
