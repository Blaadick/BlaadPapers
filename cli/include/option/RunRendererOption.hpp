// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "option/Option.hpp"

class RunRendererOption : public Option {
public:
    RunRendererOption();

    [[nodiscard]]
    int execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) override;
};
