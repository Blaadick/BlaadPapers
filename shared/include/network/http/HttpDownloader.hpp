// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "network/Downloader.hpp"
#include "network/http/HttpClient.hpp"

class HttpDownloader : public Downloader {
public:
    explicit HttpDownloader(sptr<HttpClient> httpClient);

    auto downloadFile(
        Uri uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string> override;

private:
    sptr<HttpClient> httpClient;
};
