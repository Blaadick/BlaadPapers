// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_map>
#include "WallpaperLoader.hpp"
#include "Wallpapers.hpp"
#include "option/Option.hpp"

class OptionExecutor {
public:
    explicit OptionExecutor(sptr<util::Logger> logger);

    void addOption(const std::string&, uptr<Option> option);

    int execute(int argc, char** argv);

private:
    std::unordered_map<std::string, uptr<Option>> options;
    sptr<util::Logger> logger;
};
