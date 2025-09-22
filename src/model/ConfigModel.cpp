#include "model/ConfigModel.hpp"

ConfigModel& ConfigModel::inst() {
    static ConfigModel instance;
    return instance;
}

QString ConfigModel::getConfigPath() {
    return Config::getConfigPath();
}
