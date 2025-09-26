#include "model/ConfigModel.hpp"

#include "Config.hpp"

ConfigModel& ConfigModel::inst() {
    static ConfigModel instance;
    return instance;
}

QString ConfigModel::getConfigFilePath() {
    return Config::getConfigFilePath();
}
