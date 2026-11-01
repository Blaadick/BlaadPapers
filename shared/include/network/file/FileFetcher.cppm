// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:FileFetcher;

import std;
import :Fetcher;
import :Uri;

export class FileFetcher final : public Fetcher {
public:
    auto fetchFile(Uri uri, const std::filesystem::path& destinationDir) -> std::expected<std::filesystem::path, std::string> override;
};
