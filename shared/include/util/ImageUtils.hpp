// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include <imageinfo.hpp>
#include "data/Size.hpp"

inline Size getPictureResolutionData(const std::filesystem::path& filePath) {
    const auto info = imageinfo::parse<imageinfo::FilePathReader>(filePath.c_str());
    return Size(
        static_cast<int>(info.size().width),
        static_cast<int>(info.size().height)
    );
}
