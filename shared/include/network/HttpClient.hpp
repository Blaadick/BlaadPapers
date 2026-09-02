// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <expected>
#include <filesystem>
#include <functional>
#include <string>
#include <boost/url.hpp>
#include <boost/beast/ssl.hpp>
#include "network/HeaderResponse.hpp"
#include "util/PathUtils.hpp"

class HttpClient {
public:
    HttpClient();

    [[nodiscard]]
    std::expected<std::string, std::string> requestString(const boost::url_view& url);

    [[nodiscard]]
    std::expected<std::filesystem::path, std::string> downloadFile(
        const boost::url_view& url,
        const std::filesystem::path& downloadDir = util::downloadsDir(),
        std::function<bool(const HeaderResponse&)> validateFunction = {}
    );

private:
    boost::asio::io_context ioCtx;
    boost::asio::ssl::context sslCtx;
    boost::asio::ip::tcp::resolver resolver;
};
