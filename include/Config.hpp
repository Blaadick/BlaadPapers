#pragma once

#include <QJsonObject>

class Config {
public:
    static void load();

    static QString getConfigPath();

    static QString getPostSetScriptPath();

    static QString getWallpapersPath();

    static QVector<QString> getBadTags();

private:
    static QJsonObject defaultData;
    static QJsonObject data;

    static QJsonValueRef getValue(const QString& key);
};
