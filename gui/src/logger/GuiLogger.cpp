// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "logger/GuiLogger.hpp"

#include <chrono>
#include <iostream>

using namespace std::chrono;

namespace util {
    GuiLogger::GuiLogger(sptr<StatusModel> statusModel) : statusModel(std::move(statusModel)) {}

    void GuiLogger::logInfo(const std::string_view message) const {
        auto currentTime = zoned_time(current_zone(), floor<seconds>(system_clock::now()));
        std::cout << std::format("[\033[94m{:%H:%M:%S}\033[0m] {}\n", currentTime, message);

        statusModel->sendStatus(message);
    }

    void GuiLogger::logWarning(const std::string_view message) const {
        auto currentTime = zoned_time(current_zone(), floor<seconds>(system_clock::now()));
        std::cerr << std::format("[\033[93m{:%H:%M:%S}\033[0m] {}\n", currentTime, message);

        statusModel->sendStatus(message);
    }

    void GuiLogger::logError(const std::string_view message) const {
        auto currentTime = zoned_time(current_zone(), floor<seconds>(system_clock::now()));
        std::cerr << std::format("[\033[91m{:%H:%M:%S}\033[0m] {}\n", currentTime, message);

        statusModel->sendStatus(message);
    }
}
