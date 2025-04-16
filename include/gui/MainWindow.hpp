#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    static MainWindow &getInstance();

private:
    Ui::MainWindow *ui;

    MainWindow();
};
