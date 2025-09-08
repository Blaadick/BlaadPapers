#pragma once

#include <iosfwd>
#include <QString>

inline std::ostream& operator<<(std::ostream& stream, const QString& str) {
    stream << str.toStdString();
    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const QByteArray& byteArray) {
    stream << byteArray.toStdString();
    return stream;
}
