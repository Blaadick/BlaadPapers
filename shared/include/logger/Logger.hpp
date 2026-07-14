// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>

namespace util {
    class Logger {
    public:
        virtual ~Logger() = default;

        virtual void logInfo(const std::string& message) = 0;

        virtual void logWarning(const std::string& message) = 0;

        virtual void logError(const std::string& message) = 0;
    };
}
