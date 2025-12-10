// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <print>

namespace std {
    inline void println(const QString& str) {
        std::cout << str.toStdString() << '\n';
    }

    inline void println(const QJsonDocument& json) {
        std::cout << json.toJson(QJsonDocument::Compact).toStdString() << '\n';
    }

    inline void println(const int number) {
        std::cout << number << '\n';
    }
}

namespace util {
    using namespace std::chrono;

    inline void logInfo(const char* str) {
        auto currentTime = floor<seconds>(system_clock::now());

        std::println("[{:%H:%M:%S}] {}", currentTime, str);
    }

    template<typename... T>
    void logInfo(std::format_string<T...> fmt, T&&... args) {
        auto currentTime = floor<seconds>(system_clock::now());
        auto output = std::format(fmt, std::forward<T>(args)...);

        std::println("[{:%H:%M:%S}] {}", currentTime, output);
    }

    template<typename... T>
    void logError(std::format_string<T...> fmt, T&&... args) {
        auto currentTime = floor<seconds>(system_clock::now());
        auto output = std::format(fmt, std::forward<T>(args)...);

        std::println(stderr, "\033[31m[{:%H:%M:%S}] {}\033[0m", currentTime, output);
    }
}
