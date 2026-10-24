// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <unordered_map>
#include "network/Uri.hpp"
#include "util/Pointers.hpp"

class Fetcher;

class FetchManager final {
public:
    void addFetcher(std::string scheme, uptr<Fetcher> fetcher);

    auto fetchFile(
        Uri uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string>;

private:
    std::unordered_map<std::string, uptr<Fetcher>> fetchers;
};
