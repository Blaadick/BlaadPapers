#include "model/StatusModel.hpp"

StatusModel& StatusModel::inst() {
    static StatusModel instance;
    return instance;
}

const QString& StatusModel::getStatusText() const {
    return statusText;
}

void StatusModel::setStatusText(const QString& str) {
    statusText = str;
    emit statusTextChanged();
}

int StatusModel::getRepeatCount() const {
    return repeatCount;
}

void StatusModel::increaseRepeatCount() {
    repeatCount += 1;
    emit repeatCountChanged();
}

void StatusModel::resetRepeatCount() {
    repeatCount = 1;
    emit repeatCountChanged();
}
