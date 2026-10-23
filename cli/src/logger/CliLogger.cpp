// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "logger/CliLogger.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace util {
    CliLogger::CliLogger() {
        #ifdef _WIN32
        for(const auto handle : {GetStdHandle(STD_OUTPUT_HANDLE), GetStdHandle(STD_ERROR_HANDLE)}) {
            DWORD mode = 0;
            if(GetConsoleMode(handle, &mode)) {
                SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }
        #endif
    }

    void CliLogger::logInfo(const std::string_view message) const noexcept {
        std::cout << message << "\n";
    }

    void CliLogger::logWarning(const std::string_view message) const noexcept {
        std::cerr << "\033[93m" << message << "\033[0m\n";
    }

    void CliLogger::logError(const std::string_view message) const noexcept {
        std::cerr << "\033[91m" << message << "\033[0m\n";
    }
}
