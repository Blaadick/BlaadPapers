// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/FetchManager.hpp"

#include "network/Fetcher.hpp"

void FetchManager::addFetcher(std::string scheme, uptr<Fetcher> fetcher) {
    fetchers.emplace(std::move(scheme), std::move(fetcher));
}

auto FetchManager::fetchFile(
    Uri uri,
    const std::filesystem::path& downloadDir
) -> std::expected<std::filesystem::path, std::string> {
    auto it = fetchers.find(std::string(uri.getScheme()));
    if(it == fetchers.end()) {
        return std::unexpected("Unsupported URI scheme");
    }

    return it->second->fetchFile(std::move(uri), downloadDir);
}
