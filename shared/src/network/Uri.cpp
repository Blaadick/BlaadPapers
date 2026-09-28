// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/Uri.hpp"

#include <iostream>
#include <ranges>
#include "util/StringUtils.hpp"

// TODO Fix small links parsing issue
// TODO Fix some unconventional assumptions (https://www.rfc-editor.org/info/rfc3986/)
auto Uri::parse(std::string buffer) noexcept -> std::optional<Uri> {
    auto bufferPart = buffer.subview();

    std::string_view scheme;
    auto mainSeparatorPos = bufferPart.find(':');
    if(mainSeparatorPos != std::string::npos && mainSeparatorPos != 0) {
        scheme = bufferPart.subview(0, mainSeparatorPos);
        bufferPart = bufferPart.subview(mainSeparatorPos + 1);
    } else {
        return std::nullopt;
    }

    std::optional<std::string_view> fragment;
    auto fragmentSeparatorPos = bufferPart.rfind('#');
    if(fragmentSeparatorPos != std::string::npos) {
        fragment = bufferPart.subview(fragmentSeparatorPos + 1);
        bufferPart = bufferPart.subview(0, fragmentSeparatorPos);
    }

    std::unordered_map<std::string_view, std::string_view> queries;
    auto queriesSeparatorPos = bufferPart.rfind('?');
    if(queriesSeparatorPos != std::string::npos) {
        std::string_view queriesStr;

        if(fragmentSeparatorPos != std::string::npos) {
            queriesStr = bufferPart.subview(queriesSeparatorPos + 1, fragmentSeparatorPos - queriesSeparatorPos - 1);
        } else {
            queriesStr = bufferPart.subview(queriesSeparatorPos + 1);
        }

        for(auto queryRange : queriesStr | std::views::split('&')) {
            auto query = std::string_view(queryRange.begin(), queryRange.end());

            if(!query.empty()) {
                auto queryDividerPos = query.find('=');

                queries.emplace(
                    query.subview(0, queryDividerPos),
                    queryDividerPos == std::string_view::npos ? std::string_view() : query.subview(queryDividerPos + 1)
                );
            }
        }

        bufferPart = bufferPart.subview(0, queriesSeparatorPos);
    }

    std::optional<std::string_view> authority;
    std::optional<std::string_view> path;
    if(bufferPart.starts_with("//")) {
        auto pathStartPos = bufferPart.find('/', 2);
        if(pathStartPos == std::string_view::npos) {
            authority = bufferPart.subview(2);
        } else {
            authority = bufferPart.subview(2, pathStartPos - 2);
            path = bufferPart.subview(pathStartPos);
        }
    } else {
        if(!bufferPart.empty()) {
            path = bufferPart;
        }
    }

    return Uri(
        std::move(buffer),
        std::move(scheme),
        std::move(authority),
        std::move(path),
        std::move(queries),
        std::move(fragment)
    );
}

auto Uri::string() const noexcept -> const std::string& {
    return buffer;
}

auto Uri::c_str() const noexcept -> const char* {
    return buffer.c_str();
}

auto Uri::scheme() const noexcept -> std::string_view {
    return _scheme;
}

auto Uri::authority() const noexcept -> std::optional<std::string_view> {
    return _authority;
}

auto Uri::path() const noexcept -> std::optional<std::string_view> {
    return _path;
}

auto Uri::queries() const noexcept -> std::unordered_map<std::string_view, std::string_view> {
    return _queries;
}

auto Uri::fragment() const noexcept -> std::optional<std::string_view> {
    return _fragment;
}

auto Uri::operator==(const Uri& other) const noexcept -> bool {
    return this->string() == other.string();
}

Uri::Uri(
    std::string buffer,
    std::string_view scheme,
    std::optional<std::string_view> authority,
    std::optional<std::string_view> path,
    std::unordered_map<std::string_view, std::string_view> queries,
    std::optional<std::string_view> fragment
) : buffer(std::move(buffer)), _scheme(std::move(scheme)), _authority(std::move(authority)), _path(std::move(path)), _queries(std::move(queries)), _fragment(std::move(fragment)) {}
