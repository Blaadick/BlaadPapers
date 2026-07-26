// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/InfoOption.hpp"

#include "flag/Flags.hpp"

InfoOption::InfoOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option("Shows wallpaper information"), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::vector<std::string_view> InfoOption::getUsageStrings() const {
    return {"<wallpaper_id> [flags...]"};
}

int InfoOption::execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) {
    if(arguments.empty()) {
        logger->logWarning("Wallpaper id expected");
        return 1;
    }

    const auto wallpaper = wallpapers->get(arguments[0]);
    if(!wallpaper) {
        logger->logError(std::format("Wallpaper \"{}\" not found", arguments[0]));
        return 2;
    }

    if(flags.contains(Flags::json)) {
        const auto doc = yyjson_mut_doc_new(nullptr);
        const auto root = wallpaper->yyjson_mut_wallpaper_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        logger->logInfo(yyjson_mut_write(doc, YYJSON_WRITE_NOFLAG, nullptr));
        return 0;
    }

    logger->logInfo(wallpaper->toString());
    return 0;
}
