// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/HttpClient.hpp"

std::expected<std::string, std::string> HttpClient::requestString(const boost::url_view& url) const {
    return {};
}

std::expected<std::filesystem::path, std::string> HttpClient::downloadFile(
    const boost::url_view& url,
    const std::filesystem::path& downloadDir,
    std::function<bool(const HeaderResponse&)> validateFunction
) const {
    return {};
}
