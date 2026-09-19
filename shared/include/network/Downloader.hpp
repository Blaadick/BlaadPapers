// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include "network/Uri.hpp"
#include "util/Pointers.hpp"

class DownloadManager;

class Downloader {
public:
    explicit Downloader(wptr<DownloadManager> downloadManager) : downloadManager(std::move(downloadManager)) {}

    virtual ~Downloader() = default;

    virtual auto downloadFile(
        Uri uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string> = 0;

private:
    wptr<DownloadManager> downloadManager;
};
