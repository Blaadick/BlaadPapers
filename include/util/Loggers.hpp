#pragma once

#include <iostream>
#include <print>

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
