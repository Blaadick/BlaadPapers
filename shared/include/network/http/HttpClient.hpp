// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include "network/Uri.hpp"

class HttpClient final {
public:
    HttpClient();

    ~HttpClient();

    auto requestString(const Uri& uri) -> std::expected<std::string, std::string>;

    auto downloadFile(
        const Uri& uri,
        const std::filesystem::path& downloadDir
    ) -> std::expected<std::filesystem::path, std::string>;

private:
    auto getFilename(std::string_view contentDisposition) const -> std::optional<std::string>;

    auto getFilename(const Uri& uri) const -> std::optional<std::string>;
};
