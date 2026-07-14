// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <vector>

class Option {
public:
    virtual ~Option() = default;

    [[nodiscard]]
    virtual std::string getHelpMessage() const = 0;

    [[nodiscard]]
    virtual int execute(const std::vector<std::string>& arguments) = 0;
};
