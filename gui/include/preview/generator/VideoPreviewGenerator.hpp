// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "PreviewGenerator.hpp"

class VideoPreviewGenerator final : public PreviewGenerator {
public:
    VideoPreviewGenerator();

    auto createAndSavePreview(
        const Wallpaper& wallpaper,
        const Size& previewSize,
        const std::filesystem::path& previewPath
    ) const -> std::expected<void, std::string> override;
};
