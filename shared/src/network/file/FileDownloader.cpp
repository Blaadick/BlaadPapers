// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/file/FileDownloader.hpp"

#include "util/PathUtils.hpp"

auto FileDownloader::downloadFile(Uri uri, const std::filesystem::path& downloadDir) -> std::expected<std::filesystem::path, std::string> {
    if(!util::createDirIfNotExists(downloadDir)) {
        return std::unexpected(std::format("Failed to create directory \"{}\"", downloadDir));
    }

    auto pathEncoded = uri.path();
    if(!pathEncoded.has_value()) {
        return std::unexpected("No file path recognized");
    }

    auto sourcePath = std::filesystem::path(precentDecode(*pathEncoded));
    auto destinationPath = downloadDir / sourcePath.filename();

    std::filesystem::copy(sourcePath, destinationPath);
    return destinationPath;
}
