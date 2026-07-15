// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Wallpapers.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"

class ListOption final : public Option {
public:
    ListOption(sptr<Wallpapers> wallpapers, sptr<util::Logger> logger);

    [[nodiscard]]
    std::string getHelpMessage() const override;

    [[nodiscard]]
    int execute(const std::vector<std::string_view>& arguments) override;

private:
    sptr<Wallpapers> wallpapers;
    sptr<util::Logger> logger;
};
