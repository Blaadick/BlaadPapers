// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/HelpOption.hpp"

#include <algorithm>
#include <ranges>
#include <nlohmann/json.hpp>
#include "flag/Flags.hpp"
#include "util/WallpaperUtils.hpp"

HelpOption::HelpOption(
    wptr<CliExecutor> cliExecutor,
    sptr<util::Logger> logger
) : Option("Shows all program options and flags"), cliExecutor(std::move(cliExecutor)), logger(std::move(logger)) {}

std::vector<std::string_view> HelpOption::getUsageStrings() const {
    return {"[flags...]"};
}

int HelpOption::execute(const std::vector<std::string_view>&, const std::unordered_set<sptr<Flag>>& flags) {
    const auto& options = cliExecutor.lock()->getOptions();

    if(flags.contains(Flags::json)) {
        nlohmann::json output;

        for(const auto& [name, option] : options) {
            nlohmann::json flagsJson;
            flagsJson.emplace_back(Flags::help->toJson());
            flagsJson.emplace_back(Flags::quiet->toJson());

            for(const auto& flag : option->getFlags()) {
                flagsJson.emplace_back(flag->toJson());
            }

            output["options"].emplace_back(
                nlohmann::json{
                    {"name", name},
                    {"flags", flagsJson},
                    {"description", option->getDescription()},
                }
            );
        }

        for(const auto& extension : std::views::concat(util::supportedPictureFormats, util::supportedVideoFormats)) {
            output["supported_formats"].emplace_back(extension);
        }

        logger->logInfo(output.dump());
    } else {
        logger->logInfo("Options:");

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
