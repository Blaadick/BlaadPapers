// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:HttpFetcher;

import std;
import :Fetcher;
import :HttpClient;
import :Logger;
import :Pointers;
import :Uri;

export class HttpFetcher final : public Fetcher {
public:
    HttpFetcher(sptr<HttpClient> httpClient, sptr<util::Logger> logger);

    auto fetchFile(Uri uri, const std::filesystem::path& destinationDir) -> std::expected<std::filesystem::path, std::string> override;

private:
    sptr<HttpClient> httpClient;
    sptr<util::Logger> logger;
};
