#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const MainWindow &) = delete;

    MainWindow operator=(const MainWindow &) = delete;

    static MainWindow &getInstance();

private:
    Ui::MainWindow *ui;

    MainWindow();
};
