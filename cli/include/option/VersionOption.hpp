// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "logger/Logger.hpp"
#include "option/Option.hpp"
#include "util/Pointers.hpp"

class VersionOption final : public Option {
public:
    explicit VersionOption(sptr<util::Logger> logger);

    auto getUsageStrings() const noexcept -> std::vector<std::string_view> override;

    auto execute(
        const std::vector<std::string_view>& arguments,
        const std::unordered_set<sptr<Flag>>& flags
    ) -> int override;

private:
    sptr<util::Logger> logger;
};
