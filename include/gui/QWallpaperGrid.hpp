#pragma once

#include <QGridLayout>

#include "gui/QWallpaperPreview.hpp"

class QWallpaperGrid final : public QWidget {
    Q_OBJECT

public:
    explicit QWallpaperGrid(QWidget *parent = nullptr);

    void addPreview(QWallpaperPreview *preview) const;

private:
    QGridLayout *grid;

    void resizeEvent(QResizeEvent *event) override;
};
