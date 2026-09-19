// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "option/Option.hpp"

Option::Option(std::string description) : description(std::move(description)) {}

auto Option::getDescription() const noexcept -> const std::string& {
    return description;
}

auto Option::getFlags() const noexcept -> const std::unordered_set<sptr<Flag>>& {
    return flags;
}

void Option::setFlags(std::unordered_set<sptr<Flag>> flags) noexcept {
    this->flags = std::move(flags);
}
