// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ApplyOption.hpp"

ApplyOption::ApplyOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option(), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::string ApplyOption::getHelpMessage() const {
    return "apply help";
}

int ApplyOption::execute(const std::vector<std::string>& arguments) {
    if(arguments.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    if(wallpapers->apply(arguments[0])) {
        logger->logInfo("Wallpaper \"" + arguments[0] + "\" applied");
        return 0;
    }

    logger->logError("Wallpaper \"" + arguments[0] + "\" not found");
    return 2;
}
