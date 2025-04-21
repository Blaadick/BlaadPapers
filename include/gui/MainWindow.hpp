#pragma once

#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>

#include "gui/QWallpaperGrid.hpp"

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const MainWindow &) = delete;

    MainWindow operator=(const MainWindow &) = delete;

    static MainWindow &getInstance();

    void fillWidgets() const;

private:
    QComboBox *monitorCombo;
    QLineEdit *searchBox;
    QWallpaperGrid *wallpaperGrid;

    MainWindow();
};
