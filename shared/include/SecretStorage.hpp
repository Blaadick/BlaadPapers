// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <string>

class SecretStorage final {
public:
    auto get(const std::string& name) const -> std::expected<std::string, std::string>;

    auto set(const std::string& name, const std::string& value) const -> std::expected<void, std::string>;
};
