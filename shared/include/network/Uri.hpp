// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>
#include <curl/curl.h>

class Uri final {
public:
    static auto parse(std::string buffer) noexcept -> std::optional<Uri>;

    static auto fromPath(const std::filesystem::path& path) -> Uri;

    auto string() const noexcept -> const std::string&;

    auto c_str() const noexcept -> const char*;

    auto getScheme() const noexcept -> std::string_view;

    auto getAuthority() const noexcept -> std::optional<std::string_view>;

    auto getPath() const noexcept -> std::optional<std::string_view>;

    auto getQueries() const noexcept -> std::unordered_map<std::string_view, std::string_view>;

    auto getFragment() const noexcept -> std::optional<std::string_view>;

    auto operator==(const Uri& other) const noexcept -> bool;

private:
    std::string buffer;
    std::string_view scheme;
    std::optional<std::string_view> authority;
    std::optional<std::string_view> path;
    std::unordered_map<std::string_view, std::string_view> queries;
    std::optional<std::string_view> fragment;

    Uri(
        std::string buffer,
        std::string_view scheme,
        std::optional<std::string_view> authority,
        std::optional<std::string_view> path,
        std::unordered_map<std::string_view, std::string_view> queries,
        std::optional<std::string_view> fragment
    );
};

template<>
struct std::formatter<Uri> : std::formatter<std::string_view> {
    auto format(const Uri& uri, std::format_context& ctx) const {
        return std::formatter<std::string_view>::format(uri.string(), ctx);
    }
};

template<>
struct std::hash<Uri> {
    std::size_t operator()(const Uri& uri) const noexcept {
        return std::hash<std::string>{}(uri.string());
    }
};

inline auto percentEncode(const std::string_view str) -> std::string {
    const auto encodedStr = curl_easy_escape(nullptr, str.data(), str.length());
    std::string result(encodedStr);

    curl_free(encodedStr);
    return result;
}

inline auto percentDecode(const std::string_view str) -> std::string {
    int outputLength;
    const auto decodedStr = curl_easy_unescape(nullptr, str.data(), str.length(), &outputLength);
    std::string result(decodedStr, outputLength);

    curl_free(decodedStr);
    return result;
}
