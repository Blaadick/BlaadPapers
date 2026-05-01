// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/RemoveOption.hpp"

RemoveOption::RemoveOption() : Option("remove", 'R', {}, "Removes the wallpaper") {}

int RemoveOption::execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) {
    return 0;
}
