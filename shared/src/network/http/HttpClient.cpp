// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/http/HttpClient.hpp"

#include "util/PathUtils.hpp"

HttpClient::HttpClient() {}

HttpClient::~HttpClient() {}

auto HttpClient::requestString(const Uri& uri) -> std::expected<std::string, std::string> {
    return std::unexpected("Not implemented");
}

auto HttpClient::downloadFile(const Uri& uri, const std::filesystem::path& downloadDir) -> std::expected<std::filesystem::path, std::string> {
    return std::unexpected("Not implemented");
}

auto HttpClient::getFilename(const std::string_view contentDisposition) const -> std::optional<std::string> {
    auto extractParam = [&](std::string_view paramName) -> std::string_view {
        auto pos = contentDisposition.find(paramName);
        if(pos == std::string_view::npos) {
            return {};
        }

        pos += paramName.size();
        if(pos < contentDisposition.size() && contentDisposition[pos] == '\"') {
            auto quoteEnd = contentDisposition.find('\"', pos + 1);
            return contentDisposition.substr(pos + 1, quoteEnd - pos - 1);
        }

        auto separator = contentDisposition.find(';', pos);
        return contentDisposition.substr(pos, (separator == std::string_view::npos ? contentDisposition.size() : separator) - pos);
    };

    if(auto value = extractParam("filename*="); !value.empty()) {
        auto quotePos = value.rfind('\'');

        if(quotePos != std::string_view::npos) {
            return precentDecode(value.subview(quotePos + 1));
        }

        return std::string(value);
    }

    if(auto value = extractParam("filename="); !value.empty()) {
        return std::string(value);
    }

    return std::nullopt;
}

auto HttpClient::getFilename(const Uri& uri) const -> std::optional<std::string> {
    auto path = uri.path();
    if(path.empty() || path.back().empty()) {
        return std::nullopt;
    }

    return path.back().data();
}
