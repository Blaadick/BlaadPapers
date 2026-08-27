// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace file {
    struct FileType {
        const char* mime;
        const char* extensions[];

        const char* canonicalExtension() const;
    };

    inline constexpr FileType png = {"image/png", {".png", nullptr}};
    inline constexpr FileType apng = {"image/apng", {".apng", nullptr}};
    inline constexpr FileType jpeg = {"image/jpeg", {".jpeg", ".jpg", ".jpe", ".jfif", ".jif", ".jfi", nullptr}};
    inline constexpr FileType svg = {"image/svg+xml", {".svg", ".svgz", nullptr}};
    inline constexpr FileType webp = {"image/webp", {".webp", nullptr}};
    inline constexpr FileType bmp = {"image/bmp", {".bmp", ".dib", nullptr}};
    inline constexpr FileType avif = {"image/avif", {".avif", nullptr}};
    inline constexpr FileType tiff = {"image/tiff", {".tiff", ".tif", nullptr}};
    inline constexpr FileType heif = {"image/heif", {".heif", ".heic", ".heifs", ".heics", nullptr}};
    inline constexpr FileType mp4 = {"image/mp4", {".mp4", ".m4v", ".mp4v", nullptr}};
    inline constexpr FileType webm = {"image/webm", {".webm", nullptr}};
    inline constexpr FileType mkv = {"image/matroska", {".mkv", nullptr}};

    // TODO Make constexpr
    inline const std::unordered_map<std::string_view, const FileType&> typeByMime = {
        {png.mime, png},
        {apng.mime, apng},
        {jpeg.mime, jpeg},
        {svg.mime, svg},
        {webp.mime, webp},
        {bmp.mime, bmp},
        {avif.mime, avif},
        {tiff.mime, tiff},
        {heif.mime, heif},
        {mp4.mime, mp4},
        {webm.mime, webm},
        {mkv.mime, mkv}
    };
    inline const std::unordered_map<std::string_view, const FileType&> typeByExt = {
        {".png", png},
        {".apng", apng},
        {".jpeg", jpeg},
        {".jpg", jpeg},
        {".jpe", jpeg},
        {".jfif", jpeg},
        {".jif", jpeg},
        {".jfi", jpeg},
        {".svg", svg},
        {".svgz", svg},
        {".webp", webp},
        {".bmp", bmp},
        {".dib", bmp},
        {".avif", avif},
        {".tiff", tiff},
        {".tif", tiff},
        {".heif", heif},
        {".heic", heif},
        {".heifs", heif},
        {".heics", heif},
        {".mp4", mp4},
        {".m4v", mp4},
        {".mp4v", mp4},
        {".webm", webm},
        {".mkv", mkv}
    };

    [[nodiscard]]
    std::optional<const FileType&> getTypeFromMime(std::string_view mime);

    [[nodiscard]]
    std::optional<const FileType&> getTypeFromFile(const std::filesystem::path& filePath);
}
