// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "option/Option.hpp"

class InfoOption : public Option {
public:
    InfoOption();

    [[nodiscard]]
    int execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) override;
};
