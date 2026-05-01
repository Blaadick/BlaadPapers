// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>

struct SubOption {
    const std::string name;
    const std::optional<char> shorName;
    const std::string description;
};
