// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QClipboard>
#include <QGuiApplication>

#include "model/ClipboardModel.hpp"

import blaadpapers;

void ClipboardModel::copyWallpaperDeeplink(const QString& wallpaperId) {
    QGuiApplication::clipboard()->setText(
        QString::fromStdString("blaadpapers://apply/" + percentEncode(wallpaperId.toStdString()))
    );
}
