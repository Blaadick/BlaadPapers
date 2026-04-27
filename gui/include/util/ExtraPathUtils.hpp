// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QScreen>
#include "util/PathUtils.hpp"
#include "util/ToString.hpp"

namespace util {
    inline QString getPreviewsPath(const QScreen* screen) {
        return getPreviewsPath() + '/' + toString(screen->size() * screen->devicePixelRatio());
    }
}
