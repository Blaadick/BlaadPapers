// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/DownloadManager.hpp"

#include <yyjson.h>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"
#include "network/Downloader.hpp"
#include "util/PathUtils.hpp"

void DownloadManager::addDownloader(std::string scheme, uptr<Downloader> downloader) {
    downloaders.emplace(std::move(scheme), std::move(downloader));
}

auto DownloadManager::downloadFile(Uri uri, const std::filesystem::path& downloadDir) -> std::expected<std::filesystem::path, std::string> {
    auto it = downloaders.find(std::string(uri.scheme()));
    if(it == downloaders.end()) {
        return std::unexpected("Unsupported URI scheme");
    }

    return it->second->downloadFile(std::move(uri), downloadDir);
}
