// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <boost/url.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/http/parser.hpp>
#include "util/PathUtils.hpp"

class HttpClient {
public:
    HttpClient();

    [[nodiscard]]
    std::expected<std::string, std::string> requestString(const boost::url_view& url);

    [[nodiscard]]
    std::expected<std::filesystem::path, std::string> downloadFile(
        const boost::url_view& url,
        const std::filesystem::path& downloadDir = util::downloadsDir()
    );

private:
    boost::asio::io_context ioCtx;
    boost::asio::ssl::context sslCtx;
    boost::asio::ip::tcp::resolver resolver;

    [[nodiscard]]
    std::optional<std::string> getFilename(boost::beast::string_view contentDisposition) const;

    [[nodiscard]]
    std::optional<std::string> getFilename(const boost::urls::url_view& url) const;
};
