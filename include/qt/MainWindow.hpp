#pragma once

#include <QMainWindow>

class MainWindow final : public QMainWindow {
    inline static QScreen *selectedScreen;

    MainWindow();

public:
    static QScreen *getSelectedScreen();

    static MainWindow &getInstance();
};
