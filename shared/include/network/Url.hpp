// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Url {
    std::string scheme;
    std::string domain;
    std::vector<std::string> path;
    std::unordered_map<std::string, std::string> queries;
    std::string fragment;

    static std::optional<Url> parse(std::string_view string);

    static bool isUrl(std::string_view string);

    std::string toString() const;
};

template<>
struct std::formatter<Url> : std::formatter<std::string> {
    auto format(const Url& url, std::format_context& ctx) const {
        return std::formatter<std::string>::format(url.toString(), ctx);
    }
};
