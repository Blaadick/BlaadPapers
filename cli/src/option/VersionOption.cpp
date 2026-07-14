// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/VersionOption.hpp"

#include <print>

VersionOption::VersionOption(sptr<util::Logger> logger) : Option(), logger(logger) {}

std::string VersionOption::getHelpMessage() const {
    return "version help";
}

int VersionOption::execute(const std::vector<std::string>& arguments) {
    logger->logInfo(std::format("{} {}\n{}", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION));
    return 0;
}
