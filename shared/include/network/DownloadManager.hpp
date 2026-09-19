// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <unordered_map>
#include "network/DownloadData.hpp"
#include "network/Uri.hpp"
#include "util/Pointers.hpp"

class Downloader;

class DownloadManager final {
public:
    void addDownloader(std::string scheme, uptr<Downloader> downloader);

    auto getOngoingDownloads() -> const std::unordered_map<Uri, DownloadData>&;

    void addOngoingDownload(Uri uri, DownloadData downloadData);

    void removeOngoingDownload(const Uri& uri);

    auto getOngoingDownload(const Uri& uri) const -> std::optional<const DownloadData&>;

    auto downloadFile(
        Uri uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string>;

private:
    std::unordered_map<std::string, uptr<Downloader>> downloaders;
    std::unordered_map<Uri, DownloadData> ongoingDownloads;

    static auto ongoingDownloadsFilePath() -> const std::filesystem::path&;

    void loadOngoingDownloads();

    void saveOngoingDownloads();
};
