// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:FetchManager;

import std;
import :Fetcher;
import :Pointers;
import :Uri;

export class FetchManager final {
public:
    void addFetcher(std::string scheme, uptr<Fetcher> fetcher);

    auto fetchFile(
        Uri uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string>;

private:
    std::unordered_map<std::string, uptr<Fetcher>> fetchers;
};
