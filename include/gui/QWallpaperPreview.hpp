#pragma once

#include <QAbstractButton>
#include "Wallpaper.hpp"

class QWallpaperPreview final : public QAbstractButton {
    Q_OBJECT

public:
    explicit QWallpaperPreview(const Wallpaper &wallpaper, QWidget *parent = nullptr);

private:
    const Wallpaper &wallpaper;
    QPixmap pixmap;

    QPixmap loadCachedPreview() const;

    void paintEvent(QPaintEvent *event) override;
};
