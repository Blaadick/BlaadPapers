// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>
#include <string_view>

struct Parameter {
    std::string name;
    std::optional<char> shortName;
    std::string description;

    [[nodiscard]]
    static bool isShortParameter(std::string_view string);

    [[nodiscard]]
    static bool isLongParameter(std::string_view string);

    [[nodiscard]]
    static bool isParameter(std::string_view string);
};
