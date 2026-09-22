// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QScreen>
#include "data/Size.hpp"

namespace util {
    constexpr Size getScreenAspectRatio(const QScreen* screen) {
        const auto screenGdc = std::gcd(screen->size().width(), screen->size().height());
        return Size(
            screen->size().width() / screenGdc,
            screen->size().height() / screenGdc
        );
    }
}
