#pragma once

#include <QGridLayout>
#include <QResizeEvent>
#include <QWidget>

#include "gui/QFlowLayout.hpp"
#include "gui/QWallpaperPreview.hpp"

class QWallpaperGrid final : public QWidget {
    Q_OBJECT

public:
    explicit QWallpaperGrid(QWidget *parent = nullptr);

    void addPreview(QWallpaperPreview *preview) const;

private:
    QFlowLayout *grid;
};
