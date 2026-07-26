// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/ListOption.hpp"

#include "flag/Flags.hpp"

ListOption::ListOption(
    sptr<Wallpapers> wallpapers,
    sptr<util::Logger> logger
) : Option("Shows list of all available wallpapers"), wallpapers(std::move(wallpapers)), logger(std::move(logger)) {}

std::vector<std::string_view> ListOption::getUsageStrings() const {
    return {"[flags...]"};
}

int ListOption::execute(const std::vector<std::string_view>&, const std::unordered_set<sptr<Flag>>& flags) {
    if(wallpapers->count() == 0) {
        logger->logInfo("No wallpapers");
        return 0;
    }

    std::string output;
    if(flags.contains(Flags::json)) {
        const auto doc = yyjson_mut_doc_new(nullptr);
        const auto root = yyjson_mut_arr(doc);

        for(const auto& wallpaper : *wallpapers) {
            const auto wallpaperData = wallpaper->yyjson_mut_wallpaper_obj(doc);
            yyjson_mut_arr_add_val(root, wallpaperData);
        }

        yyjson_mut_doc_set_root(doc, root);

        output = yyjson_mut_write(doc, YYJSON_WRITE_NOFLAG, nullptr);
    } else {
        for(const auto& wallpaper : *wallpapers) {
            output += wallpaper->toString() + '\n';
        }

        output.erase(output.size() - 2);
    }

    logger->logInfo(output);
    return 0;
}
