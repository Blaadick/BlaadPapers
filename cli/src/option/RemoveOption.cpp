// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/RemoveOption.hpp"

RemoveOption::RemoveOption(
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<util::Logger> logger
) : Option("Deletes the wallpaper"), wallpaperRepository(std::move(wallpaperRepository)), logger(std::move(logger)) {}

std::vector<std::string_view> RemoveOption::getUsageStrings() const {
    return {"<wallpaper_id>"};
}

int RemoveOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) {
    if(arguments.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    if(wallpaperRepository->remove(arguments[0])) {
        logger->logInfo(std::format("Wallpaper \"{}\" deleted", arguments[0]));
        return 0;
    }

    logger->logError(std::format("Wallpaper \"{}\" not found", arguments[0]));
    return 2;
}
