// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "OptionExecutor.hpp"

OptionExecutor::OptionExecutor(sptr<util::Logger> logger): logger(std::move(logger)) {}

void OptionExecutor::addOption(const std::string& name, uptr<Option> option) {
    options.emplace(name, std::move(option));
}

int OptionExecutor::execute(int argc, char** argv) {
    if(argc < 2) {
        logger->logWarning("Option expected");
        return 0;
    }

    const auto it = options.find(argv[1]);
    if(it == options.end()) {
        logger->logError("Unknown option");
        return 2;
    }

    const std::vector<std::string> arguments(argv + 2, argv + argc);
    return it->second->execute(arguments);
}
