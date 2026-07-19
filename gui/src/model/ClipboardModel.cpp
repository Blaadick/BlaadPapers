#include "model/ClipboardModel.hpp"

#include <QClipboard>
#include <QGuiApplication>

void ClipboardModel::copyWallpaperDeeplink(const QString& wallpaperId) {
    QGuiApplication::clipboard()->setText(QString("blaadpapers://apply/" + wallpaperId));
}
