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

        void logInfo(std::string_view message) const override;

        void logWarning(std::string_view message) const override;

        void logError(std::string_view message) const override;

    private:
        sptr<StatusModel> statusModel;
    };
}
