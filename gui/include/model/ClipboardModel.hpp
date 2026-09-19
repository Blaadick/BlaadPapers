// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class ClipboardModel final : public QObject {
    Q_OBJECT

public:
    Q_INVOKABLE void copyWallpaperDeeplink(const QString& wallpaperId);
};
