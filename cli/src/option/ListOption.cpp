// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ListOption.hpp"

ListOption::ListOption() : Option("list", "Shows list of all available wallpapers") {}

int ListOption::execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) {
    return 0;
}
