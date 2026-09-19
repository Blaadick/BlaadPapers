// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/Uri.hpp"

Uri::Uri(std::string str) : buffer(std::move(str)) {}

Uri::Uri(const std::string_view str) : buffer(std::string(str)) {}

Uri::Uri(const char* str) : buffer(std::string(str)) {}

bool Uri::isUri(const std::string_view str) noexcept {
    auto separatorPos = str.find(':');
    return separatorPos != std::string_view::npos && separatorPos != 0;
}

auto Uri::string() const noexcept -> const std::string& {
    return buffer;
}

auto Uri::c_str() const noexcept -> const char* {
    return buffer.c_str();
}

auto Uri::scheme() const noexcept -> std::string_view {
    return buffer.subview(0, buffer.find(':'));
}

auto Uri::authority() const noexcept -> std::optional<std::string_view> {
    auto separatorPos = buffer.find("://");
    if(separatorPos == std::string::npos) {
        return std::nullopt;
    }

    auto authorityStartPos = separatorPos + 3;
    auto authorityEndPos = buffer.find('/', authorityStartPos);

    return buffer.subview(authorityStartPos, authorityEndPos);
}

auto Uri::userInfo() const noexcept -> std::optional<std::string_view> {
    auto authority = this->authority();
    if(!authority.has_value()) {
        return std::nullopt;
    }

    auto separatorPos = authority->find('@');
    if(separatorPos == std::string_view::npos || separatorPos == 0) {
        return std::nullopt;
    }

    return authority->subview(0, separatorPos);
}

auto Uri::host() const noexcept -> std::optional<std::string_view> {
    auto authority = this->authority();
    if(!authority.has_value()) {
        return std::nullopt;
    }

    std::size_t hostStartPos = 0;
    std::size_t hostEndPos = authority->length();

    auto userInfoSeparatorPos = authority->find('@');
    if(userInfoSeparatorPos != std::string_view::npos) {
        hostStartPos = userInfoSeparatorPos + 1;
    }

    auto portSeparatorPos = authority->find(':');
    if(portSeparatorPos != std::string_view::npos) {
        hostEndPos = portSeparatorPos;
    }

    return authority->subview(hostStartPos, hostEndPos);
}

auto Uri::port() const noexcept -> std::optional<std::string_view> {
    auto authority = this->authority();
    if(!authority.has_value()) {
        return std::nullopt;
    }

    auto separatorPos = authority->find(':');
    if(separatorPos == std::string_view::npos || separatorPos == authority->length() - 1) {
        return std::nullopt;
    }

    return authority->subview(separatorPos + 1);
}

auto Uri::path() const noexcept -> std::vector<std::string_view> {
    return {};
}

auto Uri::queries() const noexcept -> std::unordered_map<std::string_view, std::string_view> {
    return {};
}

auto Uri::fragment() const noexcept -> std::optional<std::string_view> {
    return std::nullopt;
}

bool Uri::operator==(const Uri& other) const noexcept {
    return this->string() == other.string();
}
