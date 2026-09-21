// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <unordered_map>
#include "network/Uri.hpp"
#include "util/Pointers.hpp"

class Downloader;

class DownloadManager final {
public:
    void addDownloader(std::string scheme, uptr<Downloader> downloader);

    auto downloadFile(
        Uri uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string>;

private:
    std::unordered_map<std::string, uptr<Downloader>> downloaders;
};
