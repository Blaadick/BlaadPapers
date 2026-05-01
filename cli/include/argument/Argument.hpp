// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>

template<typename T>
struct Argument {
    const std::string name;
    const bool isNecessary = true;

    T parse(const char* rawArgument) const {
        return {};
    }
};
