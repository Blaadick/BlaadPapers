// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/HelpOption.hpp"

#include <algorithm>
#include <ranges>
#include "flag/Flags.hpp"

HelpOption::HelpOption(
    const std::unordered_map<std::string, uptr<Option>>& options,
    sptr<util::Logger> logger
) : Option("Shows all program options and flags"), options(options), logger(std::move(logger)) {}

std::vector<std::string_view> HelpOption::getUsageStrings() const {
    return {"[flags...]"};
}

int HelpOption::execute(const std::vector<std::string_view>&, const std::unordered_set<sptr<Flag>>& flags) {
    if(flags.contains(Flags::json)) {
        const auto doc = yyjson_mut_doc_new(nullptr);
        const auto root = yyjson_mut_obj(doc);

        const auto optionsData = yyjson_mut_arr(doc);
        for(const auto& [name, option] : options) {
            const auto optionData = yyjson_mut_obj(doc);

            const auto flagsData = yyjson_mut_arr(doc);
            yyjson_mut_arr_add_flag(doc, flagsData, &*Flags::help);
            yyjson_mut_arr_add_flag(doc, flagsData, &*Flags::quiet);

            for(const auto& flag : option->getFlags()) {
                yyjson_mut_arr_add_flag(doc, flagsData, &*flag);
            }

            yyjson_mut_obj_add_str(doc, optionData, "name", name.c_str());
            yyjson_mut_obj_add_val(doc, optionData, "flags", flagsData);
            yyjson_mut_obj_add_str(doc, optionData, "description", option->getDescription().c_str());

            yyjson_mut_arr_add_val(optionsData, optionData);
        }
        yyjson_mut_obj_add_val(doc, root, "options", optionsData);

        // const auto supportedFormatsData = yyjson_mut_arr(doc);
        // for(const auto& extension : std::views::concat(util::supportedPictureFormats, util::supportedVideoFormats)) {
        //     yyjson_mut_arr_add_str(doc, supportedFormatsData, extension.c_str());
        // }
        // yyjson_mut_obj_add_val(doc, root, "supported_formats", supportedFormatsData);

        yyjson_mut_doc_set_root(doc, root);

        logger->logInfo(yyjson_mut_write(doc, YYJSON_WRITE_NOFLAG, nullptr));
        yyjson_mut_doc_free(doc);
    } else {
        logger->logInfo("Description:");
        logger->logInfo(std::format("  {}", PROJECT_DESCRIPTION));

        logger->logInfo("\nOptions:");

        auto maxNameLength = std::ranges::max(
            options | std::views::keys | std::views::transform(&std::string::length)
        );

        for(const auto& [name, option] : options) {
            logger->logInfo(std::format("  {:<{}}  ->  {}", name, maxNameLength, option->getDescription()));
        }

        logger->logInfo("\nFlags:");
        logger->logInfo(std::format("  --{}  (-{})  ->  {}", Flags::help->name, Flags::help->shortName.value(), Flags::help->description));
        logger->logInfo(std::format("  --{} (-{})  ->  {}", Flags::quiet->name, Flags::quiet->shortName.value(), Flags::quiet->description));
    }

    return 0;
}
