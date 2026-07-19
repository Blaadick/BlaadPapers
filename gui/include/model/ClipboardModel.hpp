#pragma once

#include <QObject>

class ClipboardModel final : public QObject {
    Q_OBJECT

public:
    Q_INVOKABLE void copyWallpaperDeeplink(const QString& wallpaperId);
};
