// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "parameter/Parameter.hpp"

#include <ranges>

bool Parameter::isShortParameter(const std::string_view string) {
    return string.starts_with('-') && string.length() == 2;
}

bool Parameter::isLongParameter(const std::string_view string) {
    return string.starts_with("--") && string.length() > 2;
}

bool Parameter::isParameter(const std::string_view string) {
    return isShortParameter(string) || isLongParameter(string);
}
