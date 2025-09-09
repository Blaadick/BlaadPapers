#pragma once

#include <QJsonObject>

class Config {
public:
    static void readConfig();

    static QString getWorkingPath();

    static QVector<QString> getBadTags();

private:
    static QJsonObject defaultData;
    static QJsonObject data;

    static QJsonValueRef getValue(const QString& key);
};
