#pragma once

#include <QScreen>
#include <QStandardPaths>
#include <QString>

namespace util {
    inline QString getPreviewPath(const QScreen* screen, const QString& wallpaperName) {
        const QString previewsPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/preview/";
        const QString screenPreviewsPath = previewsPath + QString::number(screen->geometry().width() * screen->devicePixelRatio()) + "x" + QString::number(screen->geometry().height() * screen->devicePixelRatio()) + '/';

        if(QDir screenPreviewsDir(screenPreviewsPath); !screenPreviewsDir.exists()) {
            if(!screenPreviewsDir.mkpath(screenPreviewsPath)) {
                logError("Cant create previews cache dir {}", screenPreviewsPath.toStdString());
                sendStatus("Cant create previews cache dir {}", screenPreviewsPath.toStdString());
            }
        }

        return screenPreviewsPath + wallpaperName + ".webp";
    }
}
