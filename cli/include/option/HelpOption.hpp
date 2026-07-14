// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "logger/Logger.hpp"
#include "option/Option.hpp"
#include "util/Pointers.hpp"

class HelpOption final : public Option {
public:
    HelpOption(sptr<util::Logger> logger);

    [[nodiscard]]
    std::string getHelpMessage() const override;

    [[nodiscard]]
    int execute(const std::vector<std::string>& arguments) override;

private:
    sptr<util::Logger> logger;
};
