#include "model/ConfigModel.hpp"

#include "Config.hpp"

ConfigModel& ConfigModel::inst() {
    static ConfigModel instance;
    return instance;
}

QString ConfigModel::getConfigPath() {
    return Config::getConfigPath();
}
