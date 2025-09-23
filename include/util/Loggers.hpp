#pragma once

#include <iostream>
#include <print>
#include <model/StatusModel.hpp>

inline void sendStatus(const QString& str) {
    StatusModel::inst().setStatusText(str);
}

template<typename... T>
void sendStatus(std::format_string<T...> fmt, T&&... args) {
    auto formated = std::format(fmt, std::forward<T>(args)...);
    StatusModel::inst().setStatusText(QString::fromStdString(formated));
}

inline void logInfo(const char* str) {
    std::cout << str << std::endl;
}

inline void logInfo(const QString& str) {
    std::cout << str.toStdString() << std::endl;
}

inline void logInfo(const QJsonDocument& json) {
    std::cout << json.toJson(QJsonDocument::Compact).toStdString() << std::endl;
}

template<typename... T>
void logInfo(std::format_string<T...> fmt, T&&... args) {
    std::println(stdout, fmt, std::forward<T>(args)...);
}

template<typename... T>
void logError(std::format_string<T...> fmt, T&&... args) {
    std::println(stderr, fmt, std::forward<T>(args)...);
}
