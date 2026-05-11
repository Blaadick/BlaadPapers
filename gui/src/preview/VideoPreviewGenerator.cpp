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
            R"(ffmpeg -loglevel quiet -y -i "{0}" -t 5 -vf "fps=24,crop='min(iw,ih*{1}/{2}):min(ih,iw*{2}/{1})',scale={1}:{2}:flags=lanczos" -loop 0 -q:v 100 -compression_level 4 "{3}")",
            wallpaper->getFilePath().c_str(),
            previewSize.width,
            previewSize.height,
            previewPath.c_str()
        ).c_str()
    );

    return true;
}
