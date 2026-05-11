// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/CountOption.hpp"

CountOption::CountOption() : Option("count", "Shows the wallpapers count") {}

int CountOption::execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) {
    return 0;
}
