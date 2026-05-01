// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/CountOption.hpp"

CountOption::CountOption() : Option("count", 'C', {}, "Shows the wallpapers count") {}

int CountOption::execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) {
    return 0;
}
