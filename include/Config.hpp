#pragma once

#include <QJsonObject>
#include <QStandardPaths>

class Config {
public:
    static void read();

    static void update();

    static QString getConfigPath();

    static QString getPostSetScriptPath();

    static QString getWorkingPath();

    static QVector<QString> getBadTags();

private:
    static QJsonObject defaultData;
    static QJsonObject data;

    static QJsonValueRef getValue(const QString& key);
};
