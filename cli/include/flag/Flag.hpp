// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>
#include <string_view>

struct Flag {
    std::string name;
    std::optional<char> shortName;
    std::string description;

    [[nodiscard]]
    static bool isShortFlag(std::string_view string);

    [[nodiscard]]
    static bool isLongFlag(std::string_view string);

    [[nodiscard]]
    static bool isFlag(std::string_view string);
};
