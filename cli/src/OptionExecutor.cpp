// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "OptionExecutor.hpp"

int OptionExecutor::execute(const int argc, const char** argv) {
    if(argc < 2) {
        return 0;
    }

    const auto it = options.find(argv[1]);
    if(it == options.end()) {
        logger->logError("Unknown option");
        return 1;
    }

    const std::vector<std::string> arguments(argv + 2, argv + argc);
    return it->second->execute(arguments);
}
