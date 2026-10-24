// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QScreen>
#include "Size.hpp"

namespace util {
    constexpr auto getScreenAspectRatio(const QScreen* screen) -> Size {
        const auto screenGdc = std::gcd(screen->size().width(), screen->size().height());
        return Size(
            screen->size().width() / screenGdc,
            screen->size().height() / screenGdc
        );
    }
}
