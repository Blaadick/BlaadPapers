// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/StatusModel.hpp"

StatusModel::StatusModel() = default;

const QString& StatusModel::getStatusText() const {
    return statusText;
}

void StatusModel::setStatusText(const QString& str) {
    statusText = str;
    emit statusTextChanged();
}

int StatusModel::getSendCount() const {
    return sendCount;
}

void StatusModel::increaseSendCount() {
    sendCount += 1;
    emit sendCountChanged();
}

void StatusModel::resetSendCount() {
    sendCount = 1;
    emit sendCountChanged();
}

void StatusModel::sendStatus(const std::string& newStatus) {
    if(statusText == newStatus) {
        increaseSendCount();
    } else {
        setStatusText(QString::fromStdString(newStatus));
        resetSendCount();
    }
}
