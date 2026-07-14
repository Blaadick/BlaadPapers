// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/CountOption.hpp"

CountOption::CountOption(sptr<Wallpapers> wallpapers, sptr<util::Logger> logger) : Option(), wallpapers(wallpapers), logger(logger) {}

std::string CountOption::getHelpMessage() const {
    return "count help";
}

int CountOption::execute(const std::vector<std::string>& arguments) {
    logger->logInfo(std::to_string(wallpapers->count()));
    return 0;
}
