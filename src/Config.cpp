// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Config.hpp"

#include <QJsonArray>
#include "util/PathUtils.hpp"

void Config::load() {
    defaultData = {
        {"bad_tags", QJsonArray{"Sensitive", "Questionable", "Explicit"}},
        {"wallpapers_path", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/Wallpapers/"},
        {"status_bar_visible", true}
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

QString Config::getWallpapersDirPath() {
    auto path = getValue("wallpapers_path").toString();
    return path.endsWith('/') ? path : path.append('/');
}

bool Config::getStatusBarVisible() {
    return getValue("status_bar_visible").toBool();
}

void Config::setStatusBarVisible(const bool isVisible) {
    setValue("status_bar_visible", isVisible);
}

QJsonObject Config::defaultData;
QJsonObject Config::data;

QString Config::getConfigPath() {
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/blaadpapers/";
}

QJsonValueRef Config::getValue(const QString& key) {
    return data[key].isNull() ? defaultData[key] : data[key];
}

void Config::setValue(const QString& key, const QJsonValue& value) {
    data[key] = value;

    QFile configFile(getConfigFilePath());
    configFile.open(QIODeviceBase::WriteOnly);
    configFile.write(QJsonDocument(data).toJson());
    configFile.close();
}
