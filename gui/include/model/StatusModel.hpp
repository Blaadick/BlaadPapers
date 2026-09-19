// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class StatusModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString statusText READ getStatusText NOTIFY statusTextChanged)
    Q_PROPERTY(int sendCount READ getSendCount NOTIFY sendCountChanged)

public:
    StatusModel();

    auto getStatusText() const -> const QString&;

    void setStatusText(const QString& str);

    auto getSendCount() const -> int;

    void increaseSendCount();

    void resetSendCount();

    void sendStatus(std::string_view newStatus);

private:
    QString statusText;
    int sendCount = 1;

signals:
    void statusTextChanged();

    void sendCountChanged();
};
