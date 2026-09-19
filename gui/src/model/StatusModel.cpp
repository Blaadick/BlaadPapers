// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "model/StatusModel.hpp"

StatusModel::StatusModel() = default;

auto StatusModel::getStatusText() const -> const QString& {
    return statusText;
}

void StatusModel::setStatusText(const QString& str) {
    statusText = str;
    emit statusTextChanged();
}

auto StatusModel::getSendCount() const -> int {
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

void StatusModel::sendStatus(const std::string_view newStatus) {
    if(statusText == newStatus) {
        increaseSendCount();
    } else {
        setStatusText(QString::fromStdString(newStatus.data()));
        resetSendCount();
    }
}
