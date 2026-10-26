// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include "Size.hpp"
#include "file_processing/FileType.hpp"
#include "wallpaper/Wallpaper.hpp"

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
