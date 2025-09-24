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
