// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Size.hpp"

#include <format>

inline std::string Size::toString() {
    return std::format("{}x{}", width, height);
}
