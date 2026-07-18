// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "CliExecutor.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <ranges>
#include <utility>
#include "data/Url.hpp"
#include "flag/Flags.hpp"

static constexpr void printHelpMessage(
    const std::unordered_map<std::string, uptr<Option>>& options,
    const util::Logger& logger
) {
    logger.logInfo("Options:");

    auto maxNameLength = std::ranges::max(
        options | std::views::keys | std::views::transform(&std::string::length)
    );

    for(const auto& [name, option] : options) {
        logger.logInfo(std::format("  {:<{}}  ->  {}", name, maxNameLength, option->getDescription()));
    }

    logger.logInfo("\nFlags:");
    logger.logInfo(std::format("  --{}  (-{})  ->  {}", Flags::help->name, Flags::help->shortName.value(), Flags::help->description));
    logger.logInfo(std::format("  --{} (-{})  ->  {}", Flags::quiet->name, Flags::quiet->shortName.value(), Flags::quiet->description));
}

static constexpr void printOptionHelpMessage(const Option& option, const std::string_view optionName, const util::Logger& logger) {
    logger.logInfo("Description:");
    logger.logInfo(std::format("  {} - {}", optionName, option.getDescription()));

    if(!option.getUsageStrings().empty()) {
        logger.logInfo("\nUsage:");
        for(const auto& usageString : option.getUsageStrings()) {
            logger.logInfo(std::format("  blaadpapers {} {}", optionName, usageString));
        }
    }

    if(!option.getFlags().empty()) {
        logger.logInfo("\nFlags:");

        const bool hasFlagWithShortName = std::ranges::any_of(
            option.getFlags(),
            [](const auto& flag) {
                return flag->shortName.has_value();
            }
        );

        auto maxNameLength = std::ranges::max(
            option.getFlags() | std::views::transform(
                [](const auto& flag) {
                    return flag->name.length();
                }
            )
        );
        auto maxNameLengthWithoutShort = maxNameLength;

        if(hasFlagWithShortName) {
            maxNameLengthWithoutShort += 5;
        }

        for(const auto& flag : option.getFlags()) {
            if(flag->shortName.has_value()) {
                logger.logInfo(std::format("  --{:<{}} (-{})  ->  {}", flag->name, maxNameLength, flag->shortName.value(), flag->description));
            } else {
                logger.logInfo(std::format("  --{:<{}}  ->  {}", flag->name, maxNameLengthWithoutShort, flag->description));
            }
        }
    }
}

CliExecutor::CliExecutor(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void CliExecutor::addHandler(std::string domain, uptr<DeeplinkHandler> handler) {
    deeplinkHandlers.emplace(std::move(domain), std::move(handler));
}

void CliExecutor::addOption(std::string name, uptr<Option> option, const std::unordered_set<sptr<Flag>>& flags) {
    option->setFlags(flags);
    options.emplace(std::move(name), std::move(option));
}

int CliExecutor::execute(const int argc, const char** argv) {
    if(argc < 2) {
        logger->logWarning("Option or link expected");
        return 1;
    }

    std::vector<std::string_view> arguments(argv + 2, argv + argc);
    std::unordered_set<sptr<Flag>> flags;

    for(const auto& argument : arguments) {
        if(Flag::isShortFlag(argument)) {
            for(auto flag : Flags::all) {
                if(!flag->shortName.has_value()) {
                    continue;
                }

                if(argument[1] == flag->shortName) {
                    flags.emplace(std::move(flag));
                    break;
                }
            }

            continue;
        }

        if(Flag::isLongFlag(argument)) {
            for(auto flag : Flags::all) {
                if(argument.substr(2) == flag->name) {
                    flags.emplace(std::move(flag));
                    break;
                }
            }
        }
    }

    std::erase_if(arguments, Flag::isFlag);

    if(flags.contains(Flags::quiet)) {
        freopen("/dev/null/", "w", stdout);
        freopen("/dev/null/", "w", stderr);
    }

    if(const auto url = Url::parse(argv[1]); !url.has_value()) {
        if(url->scheme != "blaadpapers") {
            logger->logWarning("Only blaadpapers links supported");
            return 1;
        }

        const auto it = deeplinkHandlers.find(url->domain);
        if(it == deeplinkHandlers.end()) {
            logger->logWarning("Unknown link");
            return 1;
        }

        return it->second->handle(url.value());
    }

    if(std::strcmp(argv[1], "help") == 0) {
        printHelpMessage(options, *logger);
        return 0;
    }

    const auto it = options.find(argv[1]);
    if(it == options.end()) {
        logger->logWarning(std::format("Unknown option \"{}\"", argv[1]));
        return 1;
    }

    if(flags.contains(Flags::help)) {
        printOptionHelpMessage(*it->second, it->first, *logger);
        return 0;
    }

    return it->second->execute(arguments, flags);
}
