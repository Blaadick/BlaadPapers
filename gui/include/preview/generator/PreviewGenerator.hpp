// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <filesystem>
#include "data/Size.hpp"
#include "data/Wallpaper.hpp"
#include "util/Pointers.hpp"

class PreviewGenerator {
public:
    virtual ~PreviewGenerator() = default;

    virtual bool createAndSavePreview(
        const Wallpaper& wallpaper,
        const Size& previewSize,
        const std::filesystem::path& previewPath
    ) const = 0;
};
