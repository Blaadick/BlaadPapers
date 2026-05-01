// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ShuffleOption.hpp"

ShuffleOption::ShuffleOption() : Option("shuffle", 'S', {}, "Sets the random wallpaper") {}

int ShuffleOption::execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) {
    return 0;
}
