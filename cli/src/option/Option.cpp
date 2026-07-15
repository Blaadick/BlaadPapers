// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/Option.hpp"

Option::Option(std::string description) : description(std::move(description)) {}

const std::string& Option::getDescription() const {
    return description;
}
