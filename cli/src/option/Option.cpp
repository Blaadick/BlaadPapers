// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/Option.hpp"

Option::Option(
    const std::string& name,
    const std::optional<char>& shortName,
    const std::unordered_set<sptr<SubOption>>& availableSubOptions,
    const std::string& helpMessage
) : name(name), shortName(shortName), availableSubOptions(availableSubOptions), helpMessage(helpMessage) {}

const std::string& Option::getName() const {
    return name;
}

const std::optional<char>& Option::getShortName() const {
    return shortName;
}

const std::string& Option::getHelpString() const {
    return helpMessage;
}
