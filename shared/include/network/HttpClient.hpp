// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include "util/PathUtils.hpp"

class HttpClient {
public:
    HttpClient();

    ~HttpClient();

    [[nodiscard]]
    std::optional<std::string> requestString(std::string_view url) const;

    [[nodiscard]]
    std::optional<std::string> requestContentType(std::string_view url) const;

    /**
     * @return DownloadedFilePath
     */
    std::optional<std::filesystem::path> downloadFile(
        std::string_view url,
        const std::filesystem::path& downloadDir = util::downloadsDir(),
        std::optional<std::string_view> fileName = std::nullopt
    ) const;
};
