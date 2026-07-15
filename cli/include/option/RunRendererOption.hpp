// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Wallpapers.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"

class RunRendererOption final : public Option {
public:
    RunRendererOption(sptr<Wallpapers> wallpapers, sptr<util::Logger> logger);

    int execute(const std::vector<std::string_view>& arguments) override;

private:
    sptr<Wallpapers> wallpapers;
    sptr<util::Logger> logger;
};
