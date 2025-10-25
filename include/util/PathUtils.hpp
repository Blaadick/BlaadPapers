// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QDir>
#include <QScreen>
#include <QStandardPaths>
#include <QString>
#include "util/Loggers.hpp"

namespace util {
    inline void createDirIfNotExists(const QString& path) {
        if(const QDir dir(path); !dir.exists()) {
            if(!dir.mkpath(path)) {
                logError("Can't create directory \"{}\"", path.toStdString());
                sendStatus("Can't create directory \"{}\"", path.toStdString());
            }
        }
    }
}
