#pragma once

#include <QAbstractButton>
#include "Wallpaper.hpp"

class QWallpaperPreview final : public QAbstractButton {
    Q_OBJECT

public:
    explicit QWallpaperPreview(const Wallpaper& wallpaper, QWidget* parent = nullptr);

    void refreshPreview();

private:
    const Wallpaper& wallpaper;
    QPixmap pixmap;

    void paintEvent(QPaintEvent* event) override;
};
