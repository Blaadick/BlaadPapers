// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>
#include <QAbstractListModel>
#include <QFuture>
#include <QString>
#include <QThreadPool>
#include <QUrl>
#include <QVariant>
#include "WallpaperRepository.hpp"
#include "logger/Logger.hpp"
#include "network/FetchManager.hpp"
#include "preview/PreviewManager.hpp"
#include "wallpaper/loader/WallpaperLoaderManager.hpp"

// TODO Split & refactor
class WallpapersModel final : public QAbstractListModel {
    Q_OBJECT

public:
    enum WallpaperRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        RootDirRole,
        ResolutionRole,
        SourceRole,
        TagsRole,
        TypeRole,
        IsBadRole
    };

    WallpapersModel(
        sptr<WallpaperLoaderManager> wallpaperLoaderManager,
        sptr<WallpaperRepository> wallpaperRepository,
        sptr<FetchManager> fetchManager,
        sptr<Config> config,
        sptr<PreviewManager> previewManager,
        sptr<util::Logger> logger
    );

    void loadWallpapers();

    Q_INVOKABLE auto getFileDialogNameFilters() const -> QStringList;

    Q_INVOKABLE void downloadAndInstallWallpaperAsync(const QUrl& url);

    Q_INVOKABLE void downloadAndInstallWallpapersAsync(const QList<QUrl>& urlList);

    Q_INVOKABLE void deleteWallpaperAsync(const QString& wallpaperId);

    Q_INVOKABLE void deleteWallpapersAsync(const QStringList& wallpaperIds);

    Q_INVOKABLE void applyWallpaperAsync(const QString& wallpaperId) const;

    Q_INVOKABLE void refreshWallpapers();

    Q_INVOKABLE auto rowCount(const QModelIndex& parent) const -> int override;

    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const -> QVariant override;

    auto roleNames() const -> QHash<int, QByteArray> override;

private:
    sptr<WallpaperLoaderManager> wallpaperLoaderManager;
    sptr<WallpaperRepository> wallpaperRepository;
    sptr<FetchManager> fetchManager;
    sptr<Config> config;
    sptr<PreviewManager> previewManager;
    sptr<util::Logger> logger;

    QThreadPool deletePool;
    int runningJobs = 0;
    QList<QFuture<bool>> previewFutures;

    void downloadAndInstallWallpaper(const QUrl& url) const;

    void deleteWallpaper(const QString& wallpaperId) const;
};
