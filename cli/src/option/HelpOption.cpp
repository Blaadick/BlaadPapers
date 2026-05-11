// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/HelpOption.hpp"

HelpOption::HelpOption() : Option("help", "Shows this help") {}

int HelpOption::execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) {
    return 0;
}
