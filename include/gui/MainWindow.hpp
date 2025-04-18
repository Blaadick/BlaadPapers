#pragma once

#include <QMainWindow>

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const MainWindow &) = delete;

    MainWindow operator=(const MainWindow &) = delete;

    static MainWindow &getInstance();

private:
    MainWindow();
};
