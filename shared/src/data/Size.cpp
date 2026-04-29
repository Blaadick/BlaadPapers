// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Size.hpp"

#include <format>

std::string Size::toString() const {
    return std::format("{}x{}", width, height);
}

Size Size::operator*(const Size& right) const {
    return {width * right.width, height * right.height};
}

Size Size::operator/(const Size& right) const {
    return {width / right.width, height / right.height};
}

Size Size::operator*(const int& right) const {
    return {width * right, height * right};
}

Size Size::operator/(const int& right) const {
    return {width / right, height / right};
}
