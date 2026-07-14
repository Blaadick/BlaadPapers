// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "logger/Logger.hpp"
#include "model/StatusModel.hpp"
#include "util/Pointers.hpp"

namespace util {
    class GuiLogger final : public Logger {
    public:
        explicit GuiLogger(sptr<StatusModel> statusModel);

        void logInfo(const std::string& message) override;

        void logWarning(const std::string& message) override;

        void logError(const std::string& message) override;

    private:
        sptr<StatusModel> statusModel;
    };
}
