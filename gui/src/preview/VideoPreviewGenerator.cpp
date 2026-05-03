// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/VideoPreviewGenerator.hpp"

//TODO Use libswscale
bool VideoPreviewGenerator::createAndSavePreview(
    const uptr<Wallpaper>& wallpaper,
    const Size& previewSize,
    const std::filesystem::path& previewPath
) {
    system(
        std::format(
            R"(ffmpeg -loglevel quiet -y -i "{}" -t 5 -vf fps=24,scale={}:{}:force_original_aspect_ratio=decrease -loop 0 -lossless 1 -compression_level 6 "{}")",
            wallpaper->getFilePath().c_str(),
            previewSize.width,
            previewSize.height,
            previewPath.c_str()
        ).c_str()
    );

    return true;
}
