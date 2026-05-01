// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>
#include <unordered_set>
#include <vector>
#include "suboption/SubOption.hpp"
#include "util/Pointers.hpp"

class Option {
public:
    Option(
        const std::string& name,
        const std::optional<char>& shortName,
        const std::unordered_set<sptr<SubOption>>& availableSubOptions,
        const std::string& helpMessage
    );

    virtual ~Option() = default;

    [[nodiscard]]
    const std::string& getName() const;

    [[nodiscard]]
    const std::optional<char>& getShortName() const;

    [[nodiscard]]
    const std::string& getHelpString() const;

    [[nodiscard]]
    virtual int execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) = 0;

protected:
    const std::string name;
    const std::optional<char> shortName;
    const std::unordered_set<sptr<SubOption>> availableSubOptions;
    const std::string helpMessage;
};
