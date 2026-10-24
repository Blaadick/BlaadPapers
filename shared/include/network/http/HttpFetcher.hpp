// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "logger/Logger.hpp"
#include "network/Fetcher.hpp"
#include "network/http/HttpClient.hpp"
#include "util/Pointers.hpp"

class HttpFetcher final : public Fetcher {
public:
    HttpFetcher(sptr<HttpClient> httpClient, sptr<util::Logger> logger);

    auto fetchFile(Uri uri, const std::filesystem::path& destinationDir) -> std::expected<std::filesystem::path, std::string> override;

private:
    sptr<HttpClient> httpClient;
    sptr<util::Logger> logger;
};
