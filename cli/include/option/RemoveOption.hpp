// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "WallpaperRepository.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"

class RemoveOption final : public Option {
public:
    RemoveOption(sptr<WallpaperRepository> wallpaperRepository, sptr<util::Logger> logger);

    auto getUsageStrings() const noexcept -> std::vector<std::string_view> override;

    auto execute(
        const std::vector<std::string_view>& arguments,
        const std::unordered_set<sptr<Flag>>& flags
    ) -> int override;

private:
    sptr<WallpaperRepository> wallpaperRepository;
    sptr<util::Logger> logger;
};
