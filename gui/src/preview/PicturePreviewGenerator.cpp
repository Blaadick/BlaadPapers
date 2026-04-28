// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "preview/PicturePreviewGenerator.hpp"

#include <QImage>
#include <QScreen>
#include "util/Loggers.hpp"
#include "util/ToString.hpp"

bool PicturePreviewGenerator::createAndSavePreview(const uptr<Wallpaper>& wallpaper, QSize previewSize, const QString& previewPath, const QScreen* screen) {
    const auto preview = QImage(wallpaper->getFilePath()).scaled(
        previewSize,
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
    );

    return preview.save(previewPath, "WEBP", 100);
}
