// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>
#include <vector>

class HttpClient {
public:
    HttpClient();

    ~HttpClient();

    std::optional<std::string> requestString(std::string_view url) const;

    std::optional<std::vector<std::byte>> requestBinary(std::string_view url) const;
};
