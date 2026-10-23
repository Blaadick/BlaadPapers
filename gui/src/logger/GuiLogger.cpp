// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "logger/GuiLogger.hpp"

#include <chrono>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std::chrono;

namespace util {
    GuiLogger::GuiLogger(sptr<StatusModel> statusModel) : statusModel(std::move(statusModel)) {
        #ifdef _WIN32
        for(const auto handle : {GetStdHandle(STD_OUTPUT_HANDLE), GetStdHandle(STD_ERROR_HANDLE)}) {
            DWORD mode = 0;
            if(GetConsoleMode(handle, &mode)) {
                SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }
        #endif
    }

    void GuiLogger::logInfo(const std::string_view message) const noexcept {
        auto currentTime = zoned_time(current_zone(), round<duration<long long, std::milli>>(system_clock::now()));
        std::cout << std::format("[\033[94m{:%H:%M:%S}\033[0m] {}\n", currentTime, message);

        statusModel->sendStatus(message);
    }

    void GuiLogger::logWarning(const std::string_view message) const noexcept {
        auto currentTime = zoned_time(current_zone(), round<duration<long long, std::milli>>(system_clock::now()));
        std::cerr << std::format("[\033[93m{:%H:%M:%S}\033[0m] {}\n", currentTime, message);

        statusModel->sendStatus(message);
    }

    void GuiLogger::logError(const std::string_view message) const noexcept {
        auto currentTime = zoned_time(current_zone(), round<duration<long long, std::milli>>(system_clock::now()));
        std::cerr << std::format("[\033[91m{:%H:%M:%S}\033[0m] {}\n", currentTime, message);

        statusModel->sendStatus(message);
    }
}
