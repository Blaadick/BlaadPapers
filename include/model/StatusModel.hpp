#pragma once

#include <QObject>

class StatusModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString statusText READ getStatusText WRITE setStatusText NOTIFY statusTextChanged)

public:
    static StatusModel& inst() {
        static StatusModel instance;
        return instance;
    }

    const QString& getStatusText();

    void setStatusText(const QString& str);

signals:
    void statusTextChanged();

private:
    QString statusText;
};
