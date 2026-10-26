// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <imageinfo.hpp>
#include "Size.hpp"
#include "file_processing/FileType.hpp"

inline auto getPictureResolutionData(const std::filesystem::path& filePath) -> std::optional<Size> {
    auto fileType = file::getTypeFromFile(filePath);
    if(fileType.has_value() && &*fileType == &file::svg) {
        return Size(-1, -1);
    }

    auto info = imageinfo::parse<imageinfo::FilePathReader>(filePath.string().c_str());

    if(!info.ok()) {
        return std::nullopt;
    }

    return Size(
        static_cast<int>(info.size().width),
        static_cast<int>(info.size().height)
    );
}
