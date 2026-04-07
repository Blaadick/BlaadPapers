// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QSize>
#include <QString>

namespace util {
    inline QString toString(const QSize& size) {
        return QString::number(size.width()) + 'x' + QString::number(size.height());
    }
}
