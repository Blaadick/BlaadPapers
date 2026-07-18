// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/Option.hpp"

Option::Option(std::string description) : description(std::move(description)) {}

const std::string& Option::getDescription() const {
    return description;
}

const std::unordered_set<sptr<Flag>>& Option::getFlags() const {
    return flags;
}

void Option::setFlags(std::unordered_set<sptr<Flag>> flags) {
    this->flags = std::move(flags);
}
