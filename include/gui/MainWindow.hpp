#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow final : public QMainWindow {
public:
    static MainWindow &getInstance();

private:
    Ui::MainWindow *ui;

    MainWindow();
};
