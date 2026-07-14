// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "logger/CliLogger.hpp"

#include <iostream>

namespace util {
    void CliLogger::logInfo(const std::string& message) {
        std::cout << message << "\n";
    }

    void CliLogger::logWarning(const std::string& message) {
        std::cerr << "\033[93m" << message << "\033[0m\n";
    }

    void CliLogger::logError(const std::string& message) {
        std::cerr << "\033[91m" << message << "\033[0m\n";
    }
}
