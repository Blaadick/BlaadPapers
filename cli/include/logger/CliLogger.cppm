// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module cli:CliLogger;

import blaadpapers;
import std;

export namespace util {
    class CliLogger final : public Logger {
    public:
        CliLogger();

        void logInfo(std::string_view message) const noexcept override;

        void logWarning(std::string_view message) const noexcept override;

        void logError(std::string_view message) const noexcept override;
    };
}
