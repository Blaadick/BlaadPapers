// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "model/ConfigModel.hpp"

#include "config/Config.hpp"
#include "util/FormatUtils.hpp"

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

    auto toQString(const std::optional<std::string>& str) -> QString {
        return QString::fromStdString(str.value_or(""));
    }

    auto toOptional(const QString& str) -> std::optional<std::string> {
        auto trimmed = str.trimmed();
        if(trimmed.isEmpty()) {
            return std::nullopt;
        }

        return trimmed.toStdString();
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

auto ConfigModel::getWallhavenApiKey() const -> QString {
    return toQString(config->getWallhavenApiKey());
}

void ConfigModel::setWallhavenApiKey(const QString& newKey) {
    config->setWallhavenApiKey(toOptional(newKey));
    emit wallhavenApiKeyChanged();
}

auto ConfigModel::getDanbooruLogin() const -> QString {
    return toQString(config->getDanbooruLogin());
}

void ConfigModel::setDanbooruLogin(const QString& newLogin) {
    config->setDanbooruLogin(toOptional(newLogin));
    emit danbooruLoginChanged();
}

auto ConfigModel::getDanbooruApiKey() const -> QString {
    return toQString(config->getDanbooruApiKey());
}

void ConfigModel::setDanbooruApiKey(const QString& newKey) {
    config->setDanbooruApiKey(toOptional(newKey));
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
