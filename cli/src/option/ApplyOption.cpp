// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ApplyOption.hpp"

ApplyOption::ApplyOption() : Option("apply", 'A', {}, "Sets the wallpaper") {}

int ApplyOption::execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) {
    return 0;
}
