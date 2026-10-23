// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>
#include <unordered_set>
#include "file_processing/FileType.hpp"

namespace util {
    inline QString getFormatString(const std::unordered_set<const file::FileType*>& formats) {
        QStringList extensions;

        for(const auto* format : formats) {
            for(const auto extension : format->extensions) {
                extensions << QString("*%1").arg(QString::fromUtf8(extension));
            }
        }

        return extensions.join(' ');
    }
}
