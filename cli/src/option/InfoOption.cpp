// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/InfoOption.hpp"

InfoOption::InfoOption() : Option("info", "Shows wallpaper information") {}

int InfoOption::execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) {
    return 0;
}
