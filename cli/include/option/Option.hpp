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

    auto getDescription() const noexcept -> const std::string&;

    auto getFlags() const noexcept -> const std::unordered_set<sptr<Flag>>&;

    /**
     * Do not use this yourself. Use CliExecutor::addOption instead.
     */
    void setFlags(std::unordered_set<sptr<Flag>> flags) noexcept;

    /**
     * Don't forget to update after change option expected arguments.
     *
     * Argument variants:
     * - <name> - Necessary argument
     * - [name] - optional argument
     * - [name...] - optional arguments
     */
    virtual auto getUsageStrings() const noexcept -> std::vector<std::string_view> = 0;

    virtual auto execute(
        const std::vector<std::string_view>& arguments,
        const std::unordered_set<sptr<Flag>>& flags
    ) -> int = 0;

private:
    std::string description;
    std::unordered_set<sptr<Flag>> flags;
};
