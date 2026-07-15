// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "logger/Logger.hpp"

namespace util {
    class CliLogger final : public Logger {
    public:
        void logInfo(std::string_view message) override;

        void logWarning(std::string_view message) override;

        void logError(std::string_view message) override;
    };
}
