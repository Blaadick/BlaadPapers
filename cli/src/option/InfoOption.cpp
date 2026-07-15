// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/InfoOption.hpp"

InfoOption::InfoOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option("Shows wallpaper information"), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

int InfoOption::execute(const std::vector<std::string_view>& arguments) {
    if(arguments.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    const auto wallpaper = wallpapers->get(arguments[0]);
    if(!wallpaper) {
        logger->logError(std::format("Wallpaper \"{}\" not found", arguments[0]));
        return 2;
    }

    logger->logInfo(wallpaper->toString());
    return 0;
}
