// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/VersionOption.hpp"

#include <format>
#include <nlohmann/json.hpp>
#include "flag/Flags.hpp"

VersionOption::VersionOption(sptr<util::Logger> logger) : Option("Shows program version"), logger(std::move(logger)) {}

std::vector<std::string_view> VersionOption::getUsageStrings() const {
    return {"[flags...]"};
}

int VersionOption::execute(const std::vector<std::string_view>&, const std::unordered_set<sptr<Flag>>& flags) {
    if(flags.contains(Flags::json)) {
        const nlohmann::json outputData = {
            {"name", PROJECT_NAME},
            {"description", PROJECT_DESCRIPTION},
            {"version", PROJECT_VERSION}
        };

        logger->logInfo(outputData.dump());
    } else {
        logger->logInfo(std::format("{} {}\n{}", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION));
    }

    return 0;
}
