// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "file_processing/FileType.hpp"

auto file::FileType::canonicalExtension() const -> const char* {
    return extensions[0];
}

auto file::getTypeFromMime(const std::string_view mime) -> std::optional<const file::FileType&> {
    auto it = typeByMime.find(mime);
    if(it == typeByMime.end()) {
        return std::nullopt;
    }

    return it->second;
}

auto file::getTypeFromFile(const std::filesystem::path& filePath) -> std::optional<const file::FileType&> {
    if(!std::filesystem::is_regular_file(filePath)) {
        return std::nullopt;
    }

    auto it = typeByExt.find(filePath.extension().c_str());
    if(it == typeByExt.end()) {
        return std::nullopt;
    }

    return it->second;
}
