// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <boost/url.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/http/parser.hpp>
#include "network/DownloadData.hpp"

class HttpClient {
public:
    HttpClient();

    [[nodiscard]]
    std::expected<std::string, std::string> requestString(const boost::url_view& url);

    [[nodiscard]]
    std::expected<std::filesystem::path, std::string> downloadFile(
        const boost::url_view& url,
        const std::filesystem::path& downloadDir = localDownloadsDirPath()
    );

private:
    boost::asio::io_context ioCtx;
    boost::asio::ssl::context sslCtx;
    boost::asio::ip::tcp::resolver resolver;
    std::unordered_map<boost::url_view, DownloadData> ongoingDownloads;

    [[nodiscard]]
    static const std::filesystem::path& localDownloadsDirPath();

    [[nodiscard]]
    static const std::filesystem::path& downloadsInProgressFilePath();

    void loadOngoingDownloads();

    void saveOngoingDownloads();

    void addOngoingDownload(boost::url_view url, DownloadData downloadData);

    void removeOngoingDownload(const boost::url_view& url);

    std::optional<DownloadData> getOngoingDownload(const boost::url_view& url) const;

    [[nodiscard]]
    std::optional<std::string> getFilename(boost::beast::string_view contentDisposition) const;

    [[nodiscard]]
    std::optional<std::string> getFilename(const boost::urls::url_view& url) const;

    template<class Stream>
    std::expected<std::filesystem::path, std::string> performDownload(
        Stream& stream,
        const boost::url_view& url,
        const std::filesystem::path& downloadDir
    );
};
