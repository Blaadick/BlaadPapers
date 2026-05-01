// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/HelpOption.hpp"

HelpOption::HelpOption() : Option("help", 'H', {}, "Shows this help") {}

int HelpOption::execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) {
    return 0;
}
