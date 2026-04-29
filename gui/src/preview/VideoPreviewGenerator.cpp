// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/VideoPreviewGenerator.hpp"

#include <qprocess.h>

//TODO Use libswscale instead QProcess with ffmpeg
bool VideoPreviewGenerator::createAndSavePreview(const uptr<Wallpaper>& wallpaper, const Size& previewSize, const std::filesystem::path& previewPath) {
    QProcess ffmpeg;
    ffmpeg.start(
        "ffmpeg",
        {
            "-y",
            "-i", QString::fromStdString(wallpaper->getFilePath()),
            "-t", "5",
            "-vf", "fps=24,scale=" + QString::number(previewSize.width) + ':' + QString::number(previewSize.height) + ":force_original_aspect_ratio=decrease",
            "-loop", "0",
            "-lossless", "1",
            "-compression_level", "6",
            QString::fromStdString(previewPath)
        }
    );

    ffmpeg.waitForFinished(-1);
    return ffmpeg.exitCode() == 0;
}
