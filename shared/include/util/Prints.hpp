// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <iostream>
#include <QJsonDocument>
#include <QString>

namespace std {
    inline void println(const QString& str) {
        std::cout << str.toStdString() << '\n';
    }

    inline void println(const QJsonDocument& json) {
        std::cout << json.toJson(QJsonDocument::Compact).toStdString() << '\n';
    }

    inline void println(const int number) {
        std::cout << number << '\n';
    }
}
