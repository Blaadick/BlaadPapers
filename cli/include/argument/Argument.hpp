// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>
#include <vector>

struct Argument {
    const std::string name;
    const std::optional<char> shortName;
    const std::vector<std::string> parameters;
};
