// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "model/ConfigModel.hpp"

#include <QLocale>
#include <QTranslator>
#include "config/Config.hpp"
#include "util/PathUtils.hpp"

namespace {
    auto toQStringList(const std::vector<std::string>& strings) -> QStringList {
        QStringList list;

        for(const auto& str : strings) {
            list << QString::fromStdString(str);
        }

        return list;
    }

    auto toTags(const QStringList& list) -> std::vector<std::string> {
        std::vector<std::string> tags;

        for(const auto& str : list) {
            auto tag = str.trimmed();
            if(!tag.isEmpty()) {
                tags.emplace_back(tag.toStdString());
            }
        }

        return tags;
    }

    auto toQString(const std::expected<std::string, std::string>& str) -> QString {
        return QString::fromStdString(str.value_or(""));
    }
}

ConfigModel::ConfigModel(sptr<Config> config) : config(std::move(config)) {}

auto ConfigModel::getConfigDirUrl() const -> QUrl {
    return QUrl::fromLocalFile(QString::fromStdString(util::configDir().string()));
}

auto ConfigModel::getWallpapersPath() const -> QString {
    return QString::fromStdString(config->getWallpapersDirPath().string());
}

void ConfigModel::setWallpapersPath(const QString& newPath) {
    auto path = newPath.trimmed();
    if(!path.isEmpty()) {
        config->setWallpapersDirPath(path.toStdString());
    }

    emit wallpapersPathChanged();
}

auto ConfigModel::getDefaultTags() const -> QStringList {
    return toQStringList(config->getDefaultTags());
}

void ConfigModel::setDefaultTags(const QStringList& newTags) {
    config->setDefaultTags(toTags(newTags));
    emit defaultTagsChanged();
}

auto ConfigModel::getBadTags() const -> QStringList {
    return toQStringList(config->getBadTags());
}

void ConfigModel::setBadTags(const QStringList& newTags) {
    config->setBadTags(toTags(newTags));
    emit badTagsChanged();
}

auto ConfigModel::getDeleteWallpapersPermanently() const -> bool {
    return config->getDeleteWallpapersPermanently();
}

void ConfigModel::setDeleteWallpapersPermanently(const bool newDeletePermanently) {
    config->setDeleteWallpapersPermanently(newDeletePermanently);
    emit deleteWallpapersPermanentlyChanged();
}

auto ConfigModel::getPostInstallScript() const -> QString {
    return QString::fromStdString(config->getPostInstallScript());
}

void ConfigModel::setPostInstallScript(const QString& newScript) {
    config->setPostInstallScript(newScript.trimmed().toStdString());
    emit postInstallScriptChanged();
}

auto ConfigModel::getPostApplyScript() const -> QString {
    return QString::fromStdString(config->getPostApplyScript());
}

void ConfigModel::setPostApplyScript(const QString& newScript) {
    config->setPostApplyScript(newScript.trimmed().toStdString());
    emit postApplyScriptChanged();
}

auto ConfigModel::getPostDeleteScript() const -> QString {
    return QString::fromStdString(config->getPostDeleteScript());
}

void ConfigModel::setPostDeleteScript(const QString& newScript) {
    config->setPostDeleteScript(newScript.trimmed().toStdString());
    emit postDeleteScriptChanged();
}

auto ConfigModel::getWallhavenApiKey() const -> QString {
    return toQString(config->getWallhavenApiKey());
}

void ConfigModel::setWallhavenApiKey(const QString& newKey) {
    config->setWallhavenApiKey(newKey.trimmed().toStdString());
    emit wallhavenApiKeyChanged();
}

auto ConfigModel::getDanbooruLogin() const -> QString {
    return toQString(config->getDanbooruLogin());
}

void ConfigModel::setDanbooruLogin(const QString& newLogin) {
    config->setDanbooruLogin(newLogin.trimmed().toStdString());
    emit danbooruLoginChanged();
}

auto ConfigModel::getDanbooruApiKey() const -> QString {
    return toQString(config->getDanbooruApiKey());
}

void ConfigModel::setDanbooruApiKey(const QString& newKey) {
    config->setDanbooruApiKey(newKey.trimmed().toStdString());
    emit danbooruApiKeyChanged();
}

auto ConfigModel::getStatusBarVisible() const -> bool {
    return config->getStatusBarVisible();
}

void ConfigModel::setStatusBarVisible(const bool newVisibility) {
    config->setStatusBarVisible(newVisibility);
    emit statusBarVisibleChanged();
}

auto ConfigModel::getBadTaggedWallpapersVisible() const -> bool {
    return config->getBadTaggedWallpapersVisible();
}

void ConfigModel::setBadTaggedWallpapersVisible(const bool newVisibility) {
    config->setBadTaggedWallpapersVisible(newVisibility);
    emit badTaggedWallpapersVisibleChanged();
}

auto ConfigModel::getNativeTextRendering() const -> bool {
    return config->getNativeTextRendering();
}

void ConfigModel::setNativeTextRendering(const bool newNativeTextRendering) {
    config->setNativeTextRendering(newNativeTextRendering);
    emit nativeTextRenderingChanged();
}

auto ConfigModel::getColorScheme() const -> QString {
    return QString::fromStdString(config->getColorScheme());
}

void ConfigModel::setColorScheme(const QString& newColorScheme) {
    config->setColorScheme(newColorScheme.toStdString());
    emit colorSchemeChanged();
}

auto ConfigModel::getLanguage() const -> QString {
    return QString::fromStdString(config->getLanguage());
}

void ConfigModel::setLanguage(const QString& newLanguage) {
    config->setLanguage(newLanguage.toStdString());
    emit languageChanged();
}

auto ConfigModel::getUiLanguage() const -> QString {
    if(config->getLanguage() != "system") {
        return getLanguage();
    }

    QTranslator translator;
    if(translator.load(QLocale(), "qml", "_", ":/qt/qml/" PROJECT_NAME "/qml/i18n")) {
        return translator.language();
    }

    return "en_US";
}
