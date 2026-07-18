// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/VersionOption.hpp"

#include <format>

VersionOption::VersionOption(sptr<util::Logger> logger) : Option("Shows program version"), logger(std::move(logger)) {}

std::vector<std::string_view> VersionOption::getUsageStrings() const {
    return {"[parameters...]"};
}

int VersionOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Parameter>>& parameters) {
    logger->logInfo(std::format("{} {}\n{}", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION));
    return 0;
}
