// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

import blaadpapers;
import std;

class PreviewGenerator {
public:
    explicit PreviewGenerator(const file::FileType& previewFileType);

    virtual ~PreviewGenerator() = default;

    virtual auto createAndSavePreview(
        const Wallpaper& wallpaper,
        const Size& previewSize,
        const std::filesystem::path& previewPath
    ) const -> std::expected<void, std::string> = 0;

    auto getPreviewFileType() const noexcept -> const file::FileType&;

protected:
    const file::FileType& previewFileType;
};
