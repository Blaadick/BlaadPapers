// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Url.hpp"

#include <ranges>

std::optional<Url> Url::parse(const std::string_view string) {
    std::string scheme;
    std::string domain;
    std::vector<std::string> path;
    std::unordered_map<std::string, std::string> queries;
    std::string fragment;

    const auto dividerPos = string.find("://");
    if(dividerPos == std::string_view::npos || dividerPos == string.size() - 3) {
        return std::nullopt;
    }

    scheme = string.substr(0, dividerPos);
    auto other = string.substr(dividerPos + 3);

    const auto queriesStartPos = other.find('?');
    if(queriesStartPos != std::string_view::npos) {
        const auto queriesView = other.substr(queriesStartPos + 1);

        other = other.substr(0, queriesStartPos);

        for(const auto& queryRange : queriesView | std::views::split('&')) {
            std::string_view queryView(queryRange.begin(), queryRange.end());
            const auto queryDivider = queryView.find('=');

            queries.emplace(
                std::string(queryView.substr(0, queryDivider)),
                std::string(queryView.substr(queryDivider + 1))
            );
        }
    }

    auto first = true;
    for(const auto& pathPart : other | std::views::split('/')) {
        std::string_view pathPartView(pathPart.begin(), pathPart.end());

        if(first) {
            domain = pathPartView;
            first = false;
        } else {
            path.emplace_back(pathPartView);
        }
    }

    return Url(scheme, domain, path, queries, "");
}

bool Url::isUrl(const std::string_view string) {
    const auto dividerPos = string.find("://");
    return dividerPos != std::string_view::npos && dividerPos != string.size() - 3;
}

std::string Url::toString() const {
    auto string = std::format("{}://{}", scheme, domain);

    for(const std::string& pathPart : path) {
        string += '/' + pathPart;
    }

    if(!queries.empty()) {
        string += '?';

        for(const auto& [name, value] : queries) {
            string += std::format("{}={}&", name, value);
        }

        string.erase(string.size() - 1);
    }

    if(!fragment.empty()) {
        string += std::format("#{}", fragment);
    }

    return string;
}
