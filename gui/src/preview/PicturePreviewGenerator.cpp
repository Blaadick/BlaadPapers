// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/PicturePreviewGenerator.hpp"

#include <QImage>
#include <QScreen>

// TODO Move to libvips
bool PicturePreviewGenerator::createAndSavePreview(const uptr<Wallpaper>& wallpaper, const Size& previewSize, const std::filesystem::path& previewPath) {
    const auto preview = QImage(QString::fromStdString(wallpaper->getFilePath())).scaled(
        QSize(previewSize.width, previewSize.height),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
    );

    return preview.save(QString::fromStdString(previewPath), "WEBP", 100);
}
