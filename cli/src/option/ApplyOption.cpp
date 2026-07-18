// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ApplyOption.hpp"

ApplyOption::ApplyOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option("Sets the wallpaper"), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::vector<std::string_view> ApplyOption::getUsageStrings() const {
    return {"<wallpaper_id>"};
}

int ApplyOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Parameter>>& parameters) {
    if(arguments.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    if(wallpapers->apply(arguments[0])) {
        logger->logInfo(std::format("Wallpaper \"{}\" applied", arguments[0]));
        return 0;
    }

    logger->logError(std::format("Wallpaper \"{}\" not found", arguments[0]));
    return 2;
}
