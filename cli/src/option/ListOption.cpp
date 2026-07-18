// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ListOption.hpp"

ListOption::ListOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option("Shows list of all available wallpapers"), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::vector<std::string_view> ListOption::getUsageStrings() const {
    return {"[parameters...]"};
}

int ListOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Parameter>>& parameters) {
    if(wallpapers->count() == 0) {
        logger->logInfo("No wallpapers");
        return 0;
    }

    std::string output;
    for(const auto& wallpaper : *wallpapers) {
        output += wallpaper->toString() + '\n';
    }

    logger->logInfo(output.erase(output.size() - 2));
    return 0;
}
