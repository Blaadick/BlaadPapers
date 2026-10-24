// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/http/HttpFetcher.hpp"

HttpFetcher::HttpFetcher(sptr<HttpClient> httpClient, sptr<util::Logger> logger) : httpClient(std::move(httpClient)), logger(std::move(logger)) {}

auto HttpFetcher::fetchFile(Uri uri, const std::filesystem::path& destinationDir) -> std::expected<std::filesystem::path, std::string> {
    logger->logInfo(std::format("Downloading from \"{}\"...", uri));
    return httpClient->downloadFile(std::move(uri), destinationDir);
}
