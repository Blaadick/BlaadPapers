// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QScreen>
#include <string>
#include "util/ToString.hpp"

namespace util {
    inline std::string toString(const QScreen* screen) {
        return toString(screen->size() * screen->devicePixelRatio());
    }
}
