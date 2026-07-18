// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "logger/Logger.hpp"
#include "option/Option.hpp"
#include "util/Pointers.hpp"

class VersionOption final : public Option {
public:
    explicit VersionOption(sptr<util::Logger> logger);

    [[nodiscard]]
    std::vector<std::string_view> getUsageStrings() const override;

    int execute(
        const std::vector<std::string_view>& arguments,
        const std::unordered_set<sptr<Parameter>>& parameters
    ) override;

private:
    sptr<util::Logger> logger;
};
