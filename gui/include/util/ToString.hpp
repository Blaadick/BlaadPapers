// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QScreen>

import std;

namespace util {
    inline auto toString(const QScreen* screen) -> std::string {
        const auto pixelSize = screen->size() * screen->devicePixelRatio();
        return std::format("{}x{}", pixelSize.width(), pixelSize.height());
    }
}
