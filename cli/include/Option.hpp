// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <functional>
#include <string_view>
#include <unordered_set>

struct Option {
    std::function<void(const std::unordered_set<char>&, const std::vector<char*>&)> func;
    std::unordered_set<char> allowableSubOptions;
    std::string_view helpMessage;
};
