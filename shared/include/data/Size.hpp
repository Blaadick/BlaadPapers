// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <yyjson.h>

struct Size {
    int width = -1;
    int height = -1;

    std::string toString() const;

    Size operator*(const Size& right) const;

    Size operator/(const Size& right) const;

    Size operator*(const int& right) const;

    Size operator/(const int& right) const;
};

yyjson_mut_val* yyjson_mut_size(yyjson_mut_doc* doc, const Size* size);

bool yyjson_mut_obj_add_size(yyjson_mut_doc* doc, yyjson_mut_val* obj, const char* key, const Size* size);
