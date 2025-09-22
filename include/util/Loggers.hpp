#pragma once

#include <iostream>
#include <print>
#include <model/StatusModel.hpp>

inline void logInfo(const char* str, const bool sendStatus = false) {
    std::cout << str << std::endl;

    if(sendStatus) {
        StatusModel::inst().setStatusText(str);
    }
}

inline void logInfo(const QString& str, const bool sendStatus = false) {
    std::cout << str.toStdString() << std::endl;

    if(sendStatus) {
        StatusModel::inst().setStatusText(str);
    }
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
