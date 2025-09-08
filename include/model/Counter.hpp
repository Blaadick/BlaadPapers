#pragma once

#include <QObject>

class Counter : public QObject {
    Q_OBJECT
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)

public:
    explicit Counter(QObject* parent = nullptr);

    int getCount() const;

    Q_INVOKABLE void increment();

    Q_INVOKABLE void decrement();

    Q_INVOKABLE void reset();

signals:
    void countChanged(int newCount);

private:
    int count = 0;
};
