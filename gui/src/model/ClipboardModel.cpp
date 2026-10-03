// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "model/ClipboardModel.hpp"

#include <QClipboard>
#include <QGuiApplication>
#include "network/Uri.hpp"

void ClipboardModel::copyWallpaperDeeplink(const QString& wallpaperId) {
    QGuiApplication::clipboard()->setText(
        QString::fromStdString("blaadpapers://apply/" + precentEncode(wallpaperId.toStdString()))
    );
}
