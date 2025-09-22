#pragma once

#include "Config.hpp"

class ConfigModel : public QObject {
    Q_OBJECT

public:
    static ConfigModel& inst();

    Q_INVOKABLE static QString getConfigPath();
};
