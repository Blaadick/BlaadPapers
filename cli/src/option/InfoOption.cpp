// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/InfoOption.hpp"

InfoOption::InfoOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option(), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::string InfoOption::getHelpMessage() const {
    return "info help";
}

int InfoOption::execute(const std::vector<std::string>& arguments) {
    if(arguments.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    const auto wallpaper = wallpapers->get(arguments[0]);
    if(!wallpaper) {
        logger->logError("Wallpaper \"" + arguments[0] + "\" not found");
        return 2;
    }

    logger->logInfo(wallpaper->toString());
    return 0;
}
