// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>

struct Size {
    int width = -1;
    int height = -1;

    std::string toString() const;

    Size operator*(const Size& right) const;

    Size operator/(const Size& right) const;

    Size operator*(const int& right) const;

    Size operator/(const int& right) const;
};
