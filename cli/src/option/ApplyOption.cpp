// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ApplyOption.hpp"

ApplyOption::ApplyOption() : Option("apply", "Sets the wallpaper") {}

int ApplyOption::execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) {
    return 0;
}
