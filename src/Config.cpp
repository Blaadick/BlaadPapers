// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Config.hpp"

#include <QJsonArray>
#include "util/PathUtils.hpp"

void Config::load() {
    defaultData = {
        {"bad_tags", QJsonArray{"Sensitive", "Questionable", "Explicit"}},
        {"wallpapers_path", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/Wallpapers/"}
    };

    util::createDirIfNotExists(getConfigPath());

    if(QFile configFile(getConfigFilePath()); configFile.exists()) {
        configFile.open(QIODevice::ReadOnly);
        data = QJsonDocument::fromJson(configFile.readAll()).object();
        configFile.close();
    } else {
        configFile.open(QIODevice::WriteOnly);
        configFile.write(QJsonDocument(defaultData).toJson());
        data = defaultData;
        configFile.close();
    }

    if(QFile postSetScriptFile(getPostSetScriptFilePath()); !postSetScriptFile.exists()) {
        postSetScriptFile.open(QIODevice::WriteOnly);
        postSetScriptFile.write("wallpaperName=\"$1\"\nwallpaperFilePath=\"$2\"");
        postSetScriptFile.close();
    }
}

QString Config::getConfigFilePath() {
    return getConfigPath() + "config.json";
}

QString Config::getPostSetScriptFilePath() {
    return getConfigPath() + "post_set.sh";
}

QVector<QString> Config::getBadTags() {
    QVector<QString> badTags;

    for(auto tag : getValue("bad_tags").toArray()) {
        badTags.append(tag.toString());
    }

    return badTags;
}

QString Config::getWallpapersPath() {
    return getValue("wallpapers_path").toString();
}

QJsonObject Config::defaultData;
QJsonObject Config::data;

QString Config::getConfigPath() {
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/blaadpapers/";
}

QJsonValueRef Config::getValue(const QString& key) {
    return data[key].isNull() ? defaultData[key] : data[key];
}
