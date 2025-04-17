#pragma once

#include <QGridLayout>
#include <vector>
#include "gui/QWallpaperPreview.hpp"

class QWallpaperGrid final : public QWidget {
    Q_OBJECT

public:
    explicit QWallpaperGrid(QWidget *parent = nullptr);

    void addPreview(QWallpaperPreview *preview);

private:
    std::vector<QWallpaperPreview *> items;
    QGridLayout *grid;

    void resizeEvent(QResizeEvent *event) override;
};
