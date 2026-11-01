// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:JsonError;

import std;

export class JsonError final : public std::runtime_error {
public:
    explicit JsonError(const std::string& string);

    explicit JsonError(const char* string);
};
