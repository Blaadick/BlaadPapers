// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/RemoveOption.hpp"

RemoveOption::RemoveOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option(), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::string RemoveOption::getHelpMessage() const {
    return "remove help";
}

int RemoveOption::execute(const std::vector<std::string_view>& arguments) {
    if(arguments.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    if(wallpapers->remove(arguments[0])) {
        logger->logInfo(std::format("Wallpaper \"{}\" deleted", arguments[0]));
        return 0;
    }

    logger->logError(std::format("Wallpaper \"{}\" not found", arguments[0]));
    return 2;
}
