// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Config.hpp"

#include <QJsonArray>
#include <QStandardPaths>

#include "data/Wallpaper.hpp"
#include "util/PathUtils.hpp"

void Config::load() {
    defaultData = {
        {"bad_tags", QJsonArray{"Sensitive", "Questionable", "Explicit"}},
        {"wallpaper_paths", QJsonArray{QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/Wallpapers"}},
        {"status_bar_visible", true}
    };

    util::createDirIfNotExists(getConfigPath());

    if(QFile configFile(getConfigFilePath()); configFile.exists()) {
        util::open(configFile, QIODeviceBase::ReadOnly);
        data = QJsonDocument::fromJson(configFile.readAll()).object();
        configFile.close();
    } else {
        util::open(configFile, QIODeviceBase::WriteOnly);
        configFile.write(QJsonDocument(defaultData).toJson());
        data = defaultData;
        configFile.close();
    }

    if(QFile postSetScriptFile(getPostSetScriptFilePath()); !postSetScriptFile.exists()) {
        util::open(postSetScriptFile, QIODeviceBase::WriteOnly);
        postSetScriptFile.write("#!/bin/bash\n\nwallpaperName=\"$1\"\nwallpaperFilePath=\"$2\"\n");
        postSetScriptFile.close();
    }
}

QString Config::getConfigFilePath() {
    return getConfigPath() + "/config.json";
}

QString Config::getPostSetScriptFilePath() {
    return getConfigPath() + "/post_set.sh";
}

QVector<QString> Config::getBadTags() {
    QVector<QString> badTags;

    for(auto tag : getValue("bad_tags").toArray()) {
        badTags.append(tag.toString());
    }

    return badTags;
}

QVector<QString> Config::getWallpaperDirPaths() {
    QVector<QString> paths;

    for(auto value : getValue("wallpapers_paths").toArray()) {
        auto str = value.toString();
        if(str.endsWith('/')) {
            str.chop(1);
        }

        paths.append(str);
    }

    return paths;
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
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

QJsonValueRef Config::getValue(const QString& key) {
    return data[key].isNull() ? defaultData[key] : data[key];
}

void Config::setValue(const QString& key, const QJsonValue& value) {
    data[key] = value;

    QFile configFile(getConfigFilePath());
    util::open(configFile, QIODeviceBase::WriteOnly);
    configFile.write(QJsonDocument(data).toJson());
    configFile.close();
}
