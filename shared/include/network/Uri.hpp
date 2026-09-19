// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <format>
#include <string>
#include <unordered_map>
#include <vector>
#include <curl/curl.h>

class Uri final {
public:
    explicit Uri(std::string str);

    explicit Uri(std::string_view str);

    explicit Uri(const char* str);

    static auto isUri(std::string_view str) noexcept -> bool;

    auto string() const noexcept -> const std::string&;

    auto c_str() const noexcept -> const char*;

    auto scheme() const noexcept -> std::string_view;

    auto authority() const noexcept -> std::optional<std::string_view>;

    auto userInfo() const noexcept -> std::optional<std::string_view>;

    auto host() const noexcept -> std::optional<std::string_view>;

    auto port() const noexcept -> std::optional<std::string_view>;

    auto path() const noexcept -> std::vector<std::string_view>;

    auto queries() const noexcept -> std::unordered_map<std::string_view, std::string_view>;

    auto fragment() const noexcept -> std::optional<std::string_view>;

    bool operator==(const Uri& other) const noexcept;

private:
    std::string buffer;
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

inline std::string precentEncode(const std::string_view str) {
    const auto encodedStr = curl_easy_escape(nullptr, str.data(), str.length());
    std::string result(encodedStr);

    curl_free(encodedStr);
    return result;
}

inline std::string precentDecode(const std::string_view str) {
    int outputLength;
    const auto decodedStr = curl_easy_unescape(nullptr, str.data(), str.length(), &outputLength);
    std::string result(decodedStr, outputLength);

    curl_free(decodedStr);
    return result;
}
