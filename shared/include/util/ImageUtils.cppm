// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:ImageUtils;

import imageinfo;
import std;
import :FileType;
import :Size;

export auto getPictureResolutionData(const std::filesystem::path& filePath) -> std::optional<Size> {
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
