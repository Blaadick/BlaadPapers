// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <algorithm>
#include <array>
#include <filesystem>
#include <optional>
#include <span>
#include <string_view>

namespace file {
    namespace detail {
        template<std::size_t N>
        struct Literal {
            char value[N]{};

            consteval Literal(const char (&string)[N]) {
                std::copy_n(string, N, value);
            }

            constexpr auto view() const -> std::string_view {
                return {value, N - 1};
            }
        };
    }

    template<detail::Literal... extensions>
    inline constexpr std::array<std::string_view, sizeof...(extensions)> extensionList = {extensions.view()...};

    struct FileType {
        const std::string_view mime;
        const std::span<const std::string_view> extensions;

        consteval FileType(const std::string_view mime, const std::span<const std::string_view> extensions) : mime(mime), extensions(extensions) {
            if(!mime.contains('/')) {
                throw "MIME type must be in the type/subtype form";
            }

            if(extensions.empty()) {
                throw "File type must have at least one extension";
            }

            for(const auto extension : extensions) {
                if(!extension.starts_with('.')) {
                    throw "File extension must start with a dot";
                }
            }
        }

        FileType(const FileType&) = delete;

        constexpr auto canonicalExtension() const -> std::string_view {
            return extensions.front();
        }
    };

    inline constexpr FileType png = {"image/png", extensionList<".png">};
    inline constexpr FileType apng = {"image/apng", extensionList<".apng">};
    inline constexpr FileType jpeg = {"image/jpeg", extensionList<".jpeg", ".jpg", ".jpe", ".jfif", ".jif", ".jfi">};
    inline constexpr FileType svg = {"image/svg+xml", extensionList<".svg", ".svgz">};
    inline constexpr FileType webp = {"image/webp", extensionList<".webp">};
    inline constexpr FileType bmp = {"image/bmp", extensionList<".bmp", ".dib">};
    inline constexpr FileType avif = {"image/avif", extensionList<".avif">};
    inline constexpr FileType tiff = {"image/tiff", extensionList<".tiff", ".tif">};
    inline constexpr FileType heif = {"image/heif", extensionList<".heif", ".heic", ".heifs", ".heics">};
    inline constexpr FileType mp4 = {"video/mp4", extensionList<".mp4", ".m4v", ".mp4v">};
    inline constexpr FileType webm = {"video/webm", extensionList<".webm">};
    inline constexpr FileType mkv = {"video/x-matroska", extensionList<".mkv">};

    inline constexpr std::array allTypes = {&png, &apng, &jpeg, &svg, &webp, &bmp, &avif, &tiff, &heif, &mp4, &webm, &mkv};

    constexpr auto getTypeFromMime(const std::string_view mime) -> std::optional<const FileType&> {
        for(const auto* type : allTypes) {
            if(type->mime == mime) {
                return *type;
            }
        }

        return std::nullopt;
    }

    constexpr auto getTypeFromExtension(const std::string_view extension) -> std::optional<const FileType&> {
        for(const auto* type : allTypes) {
            if(std::ranges::contains(type->extensions, extension)) {
                return *type;
            }
        }

        return std::nullopt;
    }

    constexpr auto getTypeFromFile(const std::filesystem::path& filePath) -> std::optional<const FileType&> {
        if(!std::filesystem::is_regular_file(filePath)) {
            return std::nullopt;
        }

        return getTypeFromExtension(filePath.extension().string());
    }

    namespace detail {
        consteval auto isEachTypeUnambiguous() -> bool {
            for(const auto* type : allTypes) {
                if(&getTypeFromMime(type->mime).value() != type) {
                    return false;
                }

                for(const auto extension : type->extensions) {
                    if(&getTypeFromExtension(extension).value() != type) {
                        return false;
                    }
                }
            }

            return true;
        }
    }

    static_assert(detail::isEachTypeUnambiguous(), "Each MIME type and extension must belong to exactly one file type");
}
