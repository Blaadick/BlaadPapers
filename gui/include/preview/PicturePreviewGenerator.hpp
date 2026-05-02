// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "PreviewGenerator.hpp"

class PicturePreviewGenerator : public PreviewGenerator {
public:
    bool createAndSavePreview(
        const uptr<Wallpaper>& wallpaper,
        const Size& targetSize,
        const std::filesystem::path& targetPath
    ) override;
};
