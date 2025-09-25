#pragma once

#include <iostream>
#include <model/StatusModel.hpp>
#include <print>

namespace util {
    inline void sendStatus(const QString& newStatus) {
        if(StatusModel::inst().getStatusText() == newStatus) {
            StatusModel::inst().increaseRepeatCount();
        } else {
            StatusModel::inst().setStatusText(newStatus);
            StatusModel::inst().resetRepeatCount();
        }
    }

    template<typename... T>
    void sendStatus(std::format_string<T...> fmt, T&&... args) {
        auto formated = std::format(fmt, std::forward<T>(args)...);
        sendStatus(QString::fromStdString(formated));
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
}
