// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "option/CountOption.hpp"

CountOption::CountOption(
    sptr<WallpaperRepository> wallpaperRepository,
    sptr<util::Logger> logger
) : Option("Shows the wallpaper count"), wallpaperRepository(std::move(wallpaperRepository)), logger(std::move(logger)) {}

auto CountOption::getUsageStrings() const noexcept -> std::vector<std::string_view> {
    return {""};
}

auto CountOption::execute(
    const std::vector<std::string_view>& arguments,
    const std::unordered_set<sptr<Flag>>& flags
) -> int {
    logger->logInfo(std::to_string(wallpaperRepository->count()));
    return 0;
}
