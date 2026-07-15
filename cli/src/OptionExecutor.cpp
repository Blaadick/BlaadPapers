// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "OptionExecutor.hpp"

#include "data/Url.hpp"

OptionExecutor::OptionExecutor(sptr<util::Logger> logger) : logger(std::move(logger)) {}

void OptionExecutor::addOption(const std::string& name, uptr<Option> option) {
    options.emplace(name, std::move(option));
}

void OptionExecutor::addHandler(const std::string& domain, uptr<DeeplinkHandler> handler) {
    deeplinkHandlers.emplace(domain, std::move(handler));
}

int OptionExecutor::execute(const int argc, const char** argv) const {
    if(argc < 2) {
        logger->logWarning("Option or link expected");
        return 0;
    }

    if(Url::isUrl(argv[1])) {
        const auto url = Url::parse(argv[1]);

        if(!url.has_value()) {
            logger->logError("Failed to parse link");
            return 2;
        }

        if(url->scheme != "blaadpapers") {
            logger->logWarning("Only blaadpapers links supported");
            return 1;
        }

        const auto it = deeplinkHandlers.find(url->domain);
        if(it == deeplinkHandlers.end()) {
            logger->logError("Unknown link");
            return 2;
        }

        return it->second->handle(url.value());
    }

    const auto it = options.find(argv[1]);
    if(it == options.end()) {
        logger->logError("Unknown option");
        return 2;
    }

    const std::vector<std::string_view> arguments(argv + 2, argv + argc);
    return it->second->execute(arguments);
}
