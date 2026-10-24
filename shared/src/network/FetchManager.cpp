// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/FetchManager.hpp"

#include <yyjson.h>
#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonObj.hpp"
#include "network/Fetcher.hpp"
#include "util/PathUtils.hpp"

void FetchManager::addFetcher(std::string scheme, uptr<Fetcher> downloader) {
    downloaders.emplace(std::move(scheme), std::move(downloader));
}

auto FetchManager::FetchFile(
    Uri uri,
    const std::filesystem::path& downloadDir
) -> std::expected<std::filesystem::path, std::string> {
    auto it = downloaders.find(std::string(uri.scheme()));
    if(it == downloaders.end()) {
        return std::unexpected("Unsupported URI scheme");
    }

    return it->second->fetchFile(std::move(uri), downloadDir);
}
