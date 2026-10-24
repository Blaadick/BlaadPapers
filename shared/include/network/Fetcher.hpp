// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include "network/Uri.hpp"

class Fetcher {
public:
    virtual ~Fetcher() = default;

    virtual auto fetchFile(Uri uri, const std::filesystem::path& destinationDir) -> std::expected<std::filesystem::path, std::string> = 0;
};
