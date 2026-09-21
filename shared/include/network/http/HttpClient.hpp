// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include "network/Uri.hpp"
#include "network/http/DownloadData.hpp"

class HttpClient final {
public:
    HttpClient();

    ~HttpClient();

    auto requestString(Uri uri) -> std::expected<std::string, std::string>;

    auto downloadFile(
        Uri uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string>;

private:
    std::unordered_map<Uri, DownloadData> ongoingDownloads;

    static auto ongoingDownloadsFilePath() -> const std::filesystem::path&;

    void loadOngoingDownloads();

    void saveOngoingDownloads();

    void addOngoingDownload(Uri uri, DownloadData downloadData);

    void removeOngoingDownload(const Uri& uri);

    auto getOngoingDownload(const Uri& uri) const -> std::optional<const DownloadData&>;

    auto extractFilename(std::string_view contentDisposition) const -> std::optional<std::string>;

    auto extractFilename(const Uri& uri) const -> std::optional<std::string>;
};
