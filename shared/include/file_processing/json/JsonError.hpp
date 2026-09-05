// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

class JsonError : public std::runtime_error {
public:
    explicit JsonError(const std::string& string);

    explicit JsonError(const char* string);
};
