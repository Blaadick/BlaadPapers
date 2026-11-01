// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module cli:HelpOption;

import blaadpapers;
import std;
import :Flag;
import :Option;

export class HelpOption final : public Option {
public:
    HelpOption(
        const std::unordered_map<std::string, uptr<Option>>& options,
        sptr<util::Logger> logger
    );

    auto getUsageStrings() const noexcept -> std::vector<std::string_view> override;

    auto execute(
        const std::vector<std::string_view>& arguments,
        const std::unordered_set<sptr<Flag>>& flags
    ) -> int override;

private:
    const std::unordered_map<std::string, uptr<Option>>& options;
    sptr<util::Logger> logger;
};
