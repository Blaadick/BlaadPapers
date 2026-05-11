// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>
#include <unordered_set>
#include <vector>
#include "argument/Argument.hpp"
#include "util/Pointers.hpp"

class Option {
public:
    Option(
        const std::string& name,
        const std::string& helpMessage
    );

    virtual ~Option() = default;

    [[nodiscard]]
    const std::string& getName() const;

    [[nodiscard]]
    const std::string& getHelpString() const;

    [[nodiscard]]
    virtual int execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) = 0;

protected:
    const std::string name;
    const std::string helpMessage;
};
