// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/http/HttpDownloader.hpp"

HttpDownloader::HttpDownloader(
    wptr<DownloadManager> downloadManager,
    sptr<HttpClient> httpClient
) : Downloader(std::move(downloadManager)), httpClient(std::move(httpClient)) {}

auto HttpDownloader::downloadFile(
    Uri uri,
    const std::filesystem::path& downloadDir
) -> std::expected<std::filesystem::path, std::string> {
    return httpClient->downloadFile(std::move(uri), downloadDir);
}
