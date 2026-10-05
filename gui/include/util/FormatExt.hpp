#pragma once

#include <format>
#include <QString>

template<>
struct std::formatter<QString> : std::formatter<std::string> {
    auto format(const QString& str, std::format_context& ctx) const {
        return std::formatter<std::string>::format(str.toStdString(), ctx);
    }
};
