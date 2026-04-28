// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include <qscreen.h>

#include "data/Wallpaper.hpp"
#include "util/Pointers.hpp"

class PreviewGenerator {
public:
    virtual ~PreviewGenerator() = default;

    virtual bool createAndSavePreview(const uptr<Wallpaper>& wallpaper, QSize previewSize, const QString& previewPath, const QScreen* screen) = 0;
};
