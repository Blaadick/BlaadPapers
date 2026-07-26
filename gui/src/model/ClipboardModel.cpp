// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/ClipboardModel.hpp"

#include <QClipboard>
#include <QGuiApplication>

void ClipboardModel::copyWallpaperDeeplink(const QString& wallpaperId) {
    QGuiApplication::clipboard()->setText(QString("blaadpapers://apply/" + wallpaperId));
}
