// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include "network/HeaderResponse.hpp"
#include "network/Url.hpp"
#include "util/PathUtils.hpp"

class HttpClient {
public:
    HttpClient();

    ~HttpClient();

    [[nodiscard]]
    std::optional<std::string> requestString(const Url& url) const;

    [[nodiscard]]
    std::optional<HeaderResponse> requestHeader(const Url& url) const;

    /**
     * @return DownloadedFilePath
     */
    std::optional<std::filesystem::path> downloadFile(
        const Url& url,
        const std::filesystem::path& downloadDir = util::downloadsDir(),
        std::optional<std::string_view> fileName = std::nullopt
    ) const;
};
