// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ListOption.hpp"

ListOption::ListOption(sptr<Wallpapers> wallpapers, sptr<util::Logger> logger) : Option(), wallpapers(wallpapers), logger(logger) {}

std::string ListOption::getHelpMessage() const {
    return "list help";
}

int ListOption::execute(const std::vector<std::string>& arguments) {
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
