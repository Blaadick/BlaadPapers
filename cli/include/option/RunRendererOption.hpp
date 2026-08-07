// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "WallpaperRepository.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"

class RunRendererOption final : public Option {
public:
    RunRendererOption(sptr<WallpaperRepository> wallpaperRepository, sptr<util::Logger> logger);

    [[nodiscard]]
    std::vector<std::string_view> getUsageStrings() const override;

    [[nodiscard]]
    int execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) override;

private:
    sptr<WallpaperRepository> wallpaperRepository;
    sptr<util::Logger> logger;
};
