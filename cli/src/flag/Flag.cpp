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

nlohmann::json Flag::toJson() const {
    return {
        {"name", name},
        {"short_name", shortName.has_value() ? nlohmann::json(std::string(1, *shortName)) : nullptr},
        {"description", description},
    };
}
