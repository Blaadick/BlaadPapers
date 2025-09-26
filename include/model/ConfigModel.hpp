#pragma once

#include <QObject>

class ConfigModel : public QObject {
    Q_OBJECT

public:
    static ConfigModel& inst();

    Q_INVOKABLE QString getConfigFilePath();
};
