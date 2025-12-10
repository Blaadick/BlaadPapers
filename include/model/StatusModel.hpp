// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QObject>

class StatusModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString statusText READ getStatusText NOTIFY statusTextChanged)
    Q_PROPERTY(int repeatCount READ getRepeatCount NOTIFY repeatCountChanged)

public:
    static StatusModel& inst();

    const QString& getStatusText() const;

    void setStatusText(const QString& str);

    int getRepeatCount() const;

    void increaseRepeatCount();

    void resetRepeatCount();

signals :
    void statusTextChanged();

    void repeatCountChanged();

private:
    QString statusText;
    int repeatCount = 1;
};

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
}
