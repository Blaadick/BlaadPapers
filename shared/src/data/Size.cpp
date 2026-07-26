// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Size.hpp"

#include <format>

std::string Size::toString() const {
    return std::format("{}x{}", width, height);
}

Size Size::operator*(const Size& right) const {
    return Size(width * right.width, height * right.height);
}

Size Size::operator/(const Size& right) const {
    return Size(width / right.width, height / right.height);
}

Size Size::operator*(const int& right) const {
    return Size(width * right, height * right);
}

Size Size::operator/(const int& right) const {
    return Size(width / right, height / right);
}

yyjson_mut_val* yyjson_mut_size(yyjson_mut_doc* doc, const Size* size) {
    const auto sizeData = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_int(doc, sizeData, "width", size->width);
    yyjson_mut_obj_add_int(doc, sizeData, "height", size->height);

    return sizeData;
}

bool yyjson_mut_obj_add_size(yyjson_mut_doc* doc, yyjson_mut_val* obj, const char* key, const Size* size) {
    const auto val = yyjson_mut_size(doc, size);
    return yyjson_mut_obj_add_val(doc, obj, key, val);
}
