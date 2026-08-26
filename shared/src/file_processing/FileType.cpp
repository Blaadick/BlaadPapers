// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "file_processing/FileType.hpp"

const char* file::FileType::canonicalExtension() const {
    return extensions[0];
}

std::optional<const file::FileType&> file::getTypeFromMime(const std::string_view mime) {
    auto it = typeByMime.find(mime);
    if(it == typeByMime.end()) {
        return std::nullopt;
    }

    return it->second;
}

std::optional<const file::FileType&> file::getTypeFromFile(const std::filesystem::path& filePath) {
    if(!std::filesystem::is_regular_file(filePath)) {
        return std::nullopt;
    }

    auto it = typeByExt.find(filePath.extension().c_str());
    if(it == typeByExt.end()) {
        return std::nullopt;
    }

    return it->second;
}
