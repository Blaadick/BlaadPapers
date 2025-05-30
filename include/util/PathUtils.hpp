#pragma once

#include <filesystem>
#include <QStandardPaths>

inline std::filesystem::path getCachePath() {
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation).toStdString();
}

inline std::filesystem::path getConfigPath() {
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).toStdString();
}

inline std::filesystem::path getPicturesPath() {
    return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation).toStdString();
}
