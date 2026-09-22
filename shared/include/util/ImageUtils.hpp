// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <imageinfo.hpp>
#include "data/Size.hpp"

inline std::optional<Size> getPictureResolutionData(const std::filesystem::path& filePath) {
    auto info = imageinfo::parse<imageinfo::FilePathReader>(filePath.string().c_str());

    if(!info.ok()) {
        return std::nullopt;
    }

    return Size(
        static_cast<int>(info.size().width),
        static_cast<int>(info.size().height)
    );
}
