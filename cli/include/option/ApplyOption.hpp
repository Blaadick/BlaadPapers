// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "option/Option.hpp"

class ApplyOption : public Option {
public:
    ApplyOption();

    [[nodiscard]]
    int execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) override;
};
