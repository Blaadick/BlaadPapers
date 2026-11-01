// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <stdlib.h>
#include <curl/curl.h>

export module blaadpapers:Uri;

import std;

export class Uri final {
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
    auto operator()(const Uri& uri) const noexcept -> std::size_t {
        return std::hash<std::string>{}(uri.string());
    }
};

export auto percentEncode(const std::string_view str) -> std::string {
    const auto encodedStr = curl_easy_escape(nullptr, str.data(), str.length());
    std::string result(encodedStr);

    curl_free(encodedStr);
    return result;
}

export auto percentDecode(const std::string_view str) -> std::string {
    int outputLength;
    const auto decodedStr = curl_easy_unescape(nullptr, str.data(), str.length(), &outputLength);
    std::string result(decodedStr, outputLength);

    curl_free(decodedStr);
    return result;
}
