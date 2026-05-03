// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QString>
#include <unordered_set>

namespace util {
    inline QString getFormatString(const std::unordered_set<std::string>& formats) {
        auto it = formats.begin();
        QString formatsString = QString::fromStdString('*' + *it);

        for(auto i = 1; i < formats.size(); ++i) {
            std::advance(it, 1);
            formatsString += " *" + *it;
        }

        return formatsString;
    }
}
