#include "Config.hpp"

#include <QFile>
#include <QJsonArray>
#include <QStandardPaths>

void Config::load() {
    defaultData = {
        {"working_path", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/Wallpapers/"},
        {"bad_tags", QJsonArray{"Sensitive", "Questionable", "Explicit"}}
    };

    if(QFile configFile(getConfigPath()); configFile.exists()) {
        configFile.open(QIODevice::ReadOnly);
        data = QJsonDocument::fromJson(configFile.readAll()).object();
        configFile.close();
    } else {
        configFile.open(QIODevice::WriteOnly);
        configFile.write(QJsonDocument(defaultData).toJson());
        data = defaultData;
        configFile.close();
    }
}

QString Config::getConfigPath() {
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/blaadpapers/config.json";
}

QString Config::getPostSetScriptPath() {
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/blaadpapers/post_set.sh";
}

QString Config::getWorkingPath() {
    return getValue("working_path").toString();
}

QVector<QString> Config::getBadTags() {
    QVector<QString> badTags;

    for(auto tag : getValue("bad_tags").toArray()) {
        badTags.append(tag.toString());
    }

    return badTags;
}

QJsonObject Config::defaultData;
QJsonObject Config::data;

QJsonValueRef Config::getValue(const QString& key) {
    return data[key].isNull() ? defaultData[key] : data[key];
}
