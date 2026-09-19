// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "logger/Logger.hpp"

namespace util {
    class CliLogger final : public Logger {
    public:
        void logInfo(std::string_view message) const noexcept override;

        void logWarning(std::string_view message) const noexcept override;

        void logError(std::string_view message) const noexcept override;
    };
}
