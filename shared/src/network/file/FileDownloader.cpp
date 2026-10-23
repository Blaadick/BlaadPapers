// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "network/file/FileDownloader.hpp"

#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

auto FileDownloader::downloadFile(Uri uri, const std::filesystem::path& downloadDir) -> std::expected<std::filesystem::path, std::string> {
    if(!util::createDirIfNotExists(downloadDir)) {
        return std::unexpected(std::format("Failed to create directory \"{}\"", downloadDir));
    }

    auto pathEncoded = uri.path();
    if(!pathEncoded.has_value()) {
        return std::unexpected("No file path recognized");
    }

    auto sourcePathStr = precentDecode(*pathEncoded);

    #ifdef _WIN32
    if(sourcePathStr.size() > 2 && sourcePathStr[0] == '/' && sourcePathStr[2] == ':') {
        sourcePathStr.erase(0, 1);
    }
    #endif

    auto sourcePath = fs::path(sourcePathStr);
    auto destinationPath = downloadDir / sourcePath.filename();

    std::error_code errorCode;
    if(!fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing, errorCode)) {
        return std::unexpected(std::format("Failed to copy file ({})", errorCode.message()));
    }

    return destinationPath;
}
