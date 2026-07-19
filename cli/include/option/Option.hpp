// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include "flag/Flag.hpp"
#include "util/Pointers.hpp"

class Option {
public:
    explicit Option(std::string description);

    virtual ~Option() = default;

    [[nodiscard]]
    const std::string& getDescription() const;

    [[nodiscard]]
    const std::unordered_set<sptr<Flag>>& getFlags() const;

    /**
     * Do not use this yourself. Use CliExecutor::addOption instead.
     */
    void setFlags(std::unordered_set<sptr<Flag>> flags);

    /**
     * Don't forget to update after change option expected arguments.
     *
     * Argument variants:
     * - <name> - Necessary argument
     * - [name] - optional argument
     * - [name...] - optional arguments
     */
    [[nodiscard]]
    virtual std::vector<std::string_view> getUsageStrings() const = 0;

    [[nodiscard]]
    virtual int execute(const std::vector<std::string_view>& arguments, const std::unordered_set<sptr<Flag>>& flags) = 0;

private:
    std::string description;
    std::unordered_set<sptr<Flag>> flags;
};
