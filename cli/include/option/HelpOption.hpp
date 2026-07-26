// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "CliExecutor.hpp"
#include "option/Option.hpp"

// TODO Change CliExecutor to map of options
class HelpOption final : public Option {
public:
    HelpOption(const std::unordered_map<std::string, uptr<Option>>& options, sptr<util::Logger> logger);

    [[nodiscard]]
    std::vector<std::string_view> getUsageStrings() const override;

    [[nodiscard]]
    int execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) override;

private:
    const std::unordered_map<std::string, uptr<Option>>& options;
    sptr<util::Logger> logger;
};
