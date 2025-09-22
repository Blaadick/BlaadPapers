#include "model/StatusModel.hpp"

const QString& StatusModel::getStatusText() {
    return statusText;
}

void StatusModel::setStatusText(const QString& str) {
    statusText = str;
    emit statusTextChanged();
}
