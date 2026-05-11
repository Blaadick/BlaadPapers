// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/Option.hpp"

Option::Option(
    const std::string& name,
    const std::string& helpMessage
) : name(name), helpMessage(helpMessage) {}

const std::string& Option::getName() const {
    return name;
}

const std::string& Option::getHelpString() const {
    return helpMessage;
}
