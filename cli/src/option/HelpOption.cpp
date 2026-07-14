// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/HelpOption.hpp"

HelpOption::HelpOption(sptr<util::Logger> logger) : Option(), logger(logger) {}

std::string HelpOption::getHelpMessage() const {
    return "help help";
}

int HelpOption::execute(const std::vector<std::string>& arguments) {
    logger->logInfo("Some general help message");
    return 0;
}
