// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewGenerator.hpp"

class PicturePreviewGenerator final : public PreviewGenerator {
public:
    PicturePreviewGenerator();

    auto createAndSavePreview(
        const Wallpaper& wallpaper,
        const Size& previewSize,
        const std::filesystem::path& previewPath
    ) const -> std::expected<void, std::string> override;
};
