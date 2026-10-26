// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QUrl>
#include "config/Config.hpp"

class ConfigModel final : public QObject {
    Q_OBJECT
    Q_PROPERTY(QUrl configDirUrl READ getConfigDirUrl CONSTANT)
    Q_PROPERTY(QString wallpapersPath READ getWallpapersPath WRITE setWallpapersPath NOTIFY wallpapersPathChanged)
    Q_PROPERTY(QStringList defaultTags READ getDefaultTags WRITE setDefaultTags NOTIFY defaultTagsChanged)
    Q_PROPERTY(QStringList badTags READ getBadTags WRITE setBadTags NOTIFY badTagsChanged)
    Q_PROPERTY(QString postInstallScript READ getPostInstallScript WRITE setPostInstallScript NOTIFY postInstallScriptChanged)
    Q_PROPERTY(QString postApplyScript READ getPostApplyScript WRITE setPostApplyScript NOTIFY postApplyScriptChanged)
    Q_PROPERTY(QString postDeleteScript READ getPostDeleteScript WRITE setPostDeleteScript NOTIFY postDeleteScriptChanged)
    Q_PROPERTY(QString wallhavenApiKey READ getWallhavenApiKey WRITE setWallhavenApiKey NOTIFY wallhavenApiKeyChanged)
    Q_PROPERTY(QString danbooruLogin READ getDanbooruLogin WRITE setDanbooruLogin NOTIFY danbooruLoginChanged)
    Q_PROPERTY(QString danbooruApiKey READ getDanbooruApiKey WRITE setDanbooruApiKey NOTIFY danbooruApiKeyChanged)
    Q_PROPERTY(bool statusBarVisible READ getStatusBarVisible WRITE setStatusBarVisible NOTIFY statusBarVisibleChanged)
    Q_PROPERTY(bool badTaggedWallpapersVisible READ getBadTaggedWallpapersVisible WRITE setBadTaggedWallpapersVisible NOTIFY badTaggedWallpapersVisibleChanged)
    Q_PROPERTY(bool nativeTextRendering READ getNativeTextRendering WRITE setNativeTextRendering NOTIFY nativeTextRenderingChanged)
    Q_PROPERTY(QString colorScheme READ getColorScheme WRITE setColorScheme NOTIFY colorSchemeChanged)

public:
    explicit ConfigModel(sptr<Config> config);

    auto getConfigDirUrl() const -> QUrl;

    auto getWallpapersPath() const -> QString;

    void setWallpapersPath(const QString& newPath);

    auto getDefaultTags() const -> QStringList;

    void setDefaultTags(const QStringList& newTags);

    auto getBadTags() const -> QStringList;

    void setBadTags(const QStringList& newTags);

    auto getPostInstallScript() const -> QString;

    void setPostInstallScript(const QString& newScript);

    auto getPostApplyScript() const -> QString;

    void setPostApplyScript(const QString& newScript);

    auto getPostDeleteScript() const -> QString;

    void setPostDeleteScript(const QString& newScript);

    auto getWallhavenApiKey() const -> QString;

    void setWallhavenApiKey(const QString& newKey);

    auto getDanbooruLogin() const -> QString;

    void setDanbooruLogin(const QString& newLogin);

    auto getDanbooruApiKey() const -> QString;

    void setDanbooruApiKey(const QString& newKey);

    auto getStatusBarVisible() const -> bool;

    void setStatusBarVisible(bool newVisibility);

    auto getBadTaggedWallpapersVisible() const -> bool;

    void setBadTaggedWallpapersVisible(bool newVisibility);

    auto getNativeTextRendering() const -> bool;

    void setNativeTextRendering(bool newNativeTextRendering);

    auto getColorScheme() const -> QString;

    void setColorScheme(const QString& newColorScheme);

private:
    sptr<Config> config;

signals:
    void wallpapersPathChanged();

    void defaultTagsChanged();

    void badTagsChanged();

    void postInstallScriptChanged();

    void postApplyScriptChanged();

    void postDeleteScriptChanged();

    void wallhavenApiKeyChanged();

    void danbooruLoginChanged();

    void danbooruApiKeyChanged();

    void statusBarVisibleChanged();

    void badTaggedWallpapersVisibleChanged();

    void nativeTextRenderingChanged();

    void colorSchemeChanged();
};
