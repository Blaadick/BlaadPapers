// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "logger/Logger.hpp"

namespace util {
    class CliLogger final : public Logger {
    public:
        void logInfo(const std::string& message) override;

        void logWarning(const std::string& message) override;

        void logError(const std::string& message) override;
    };
}
