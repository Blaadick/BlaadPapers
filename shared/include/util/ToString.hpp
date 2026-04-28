// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QSize>

namespace util {
    inline std::string toString(const QSize& size) {
        return std::format("{}x{}", size.width(), size.height());
    }
}
