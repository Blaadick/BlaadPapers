// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "CliExecutor.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <ranges>
#include <utility>
#include "data/Url.hpp"

static constexpr void printHelpMessage(
    const std::unordered_map<std::string, uptr<Option>>& options,
    const Parameter& helpParam,
    const Parameter& quietParam,
    const util::Logger& logger
) {
    logger.logInfo("Options:");

    auto maxNameLength = std::ranges::max(
        options | std::views::keys | std::views::transform(&std::string::length)
    );

    for(const auto& [name, option] : options) {
        logger.logInfo(std::format("  {:<{}}  ->  {}", name, maxNameLength, option->getDescription()));
    }

    logger.logInfo("\nParameters:");
    logger.logInfo(std::format("  --{}  (-{})  ->  {}", helpParam.name, helpParam.shortName.value(), helpParam.description));
    logger.logInfo(std::format("  --{} (-{})  ->  {}", quietParam.name, quietParam.shortName.value(), quietParam.description));
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

    if(!option.getParameters().empty()) {
        logger.logInfo("\nParameters:");

        const bool hasParameterWithShortName = std::ranges::any_of(
            option.getParameters(),
            [](const auto& param) {
                return param->shortName.has_value();
            }
        );

        auto maxNameLength = std::ranges::max(
            option.getParameters() | std::views::transform(
                [](const auto& param) {
                    return param->name.length();
                }
            )
        );
        auto maxNameLengthWithShort = maxNameLength;

        if(hasParameterWithShortName) {
            maxNameLength += 5;
        }

        for(const auto& param : option.getParameters()) {
            if(param->shortName.has_value()) {
                logger.logInfo(std::format("  --{:<{}} (-{})  ->  {}", param->name, maxNameLengthWithShort, param->shortName.value(), param->description));
            } else {
                logger.logInfo(std::format("  --{:<{}}  ->  {}", param->name, maxNameLength, param->description));
            }
        }
    }
}

CliExecutor::CliExecutor(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void CliExecutor::addHandler(std::string domain, uptr<DeeplinkHandler> handler) {
    deeplinkHandlers.emplace(std::move(domain), std::move(handler));
}

void CliExecutor::addOption(std::string name, uptr<Option> option, std::unordered_set<sptr<Parameter>> parameters) {
    option->setParameters(parameters);
    options.emplace(std::move(name), std::move(option));
    allParameters.merge(parameters);
}

int CliExecutor::execute(const int argc, const char** argv) {
    if(argc < 2) {
        logger->logWarning("Option or link expected");
        return 1;
    }

    const auto helpParam = std::make_shared<Parameter>("help", 'h', "Displays help page for option");
    const auto quietParam = std::make_shared<Parameter>("quiet", 'q', "Suppress whole command output");
    allParameters.emplace(helpParam);
    allParameters.emplace(quietParam);

    std::vector<std::string_view> arguments(argv + 2, argv + argc);
    std::unordered_set<sptr<Parameter>> parameters;

    for(const auto& argument : arguments) {
        if(Parameter::isShortParameter(argument)) {
            for(auto parameter : allParameters) {
                if(!parameter->shortName.has_value()) {
                    continue;
                }

                if(argument[1] == parameter->shortName) {
                    parameters.emplace(std::move(parameter));
                    break;
                }
            }

            continue;
        }

        if(Parameter::isLongParameter(argument)) {
            for(auto parameter : allParameters) {
                if(argument.substr(2) == parameter->name) {
                    parameters.emplace(std::move(parameter));
                    break;
                }
            }
        }
    }

    std::erase_if(arguments, Parameter::isParameter);

    if(parameters.contains(quietParam)) {
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
        printHelpMessage(options, *helpParam, *quietParam, *logger);
        return 0;
    }

    const auto it = options.find(argv[1]);
    if(it == options.end()) {
        logger->logWarning(std::format("Unknown option \"{}\"", argv[1]));
        return 1;
    }

    if(parameters.contains(helpParam)) {
        printOptionHelpMessage(*it->second, it->first, *logger);
        return 0;
    }

    return it->second->execute(arguments, parameters);
}
