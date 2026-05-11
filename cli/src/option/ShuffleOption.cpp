// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ShuffleOption.hpp"

ShuffleOption::ShuffleOption() : Option("shuffle", "Sets the random wallpaper") {}

int ShuffleOption::execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) {
    return 0;
}
