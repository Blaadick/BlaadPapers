// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <format>
#include <QScreen>
#include <string>

namespace util {
    inline std::string toString(const QScreen* screen) {
        const auto pixelSize = screen->size() * screen->devicePixelRatio();
        return std::format("{}x{}", pixelSize.width(), pixelSize.height());
    }
}
