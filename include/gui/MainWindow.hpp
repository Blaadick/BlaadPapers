#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow final : public QMainWindow {
    QScreen *selectedScreen;
    Ui::MainWindow *ui;

public:
    MainWindow();
    ~MainWindow() override;
};
