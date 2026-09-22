// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QVariant>
#include "WallpaperRepository.hpp"
#include "logger/Logger.hpp"
#include "preview/PreviewManager.hpp"
#include "wallpaper_loader/WallpaperLoaderManager.hpp"

class WallpapersModel : public QAbstractListModel {
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
        sptr<WallpaperLoaderManager> wallpaperLoader,
        sptr<WallpaperRepository> wallpaperRepository,
        sptr<Config> config,
        sptr<PreviewManager> previewManager,
        sptr<util::Logger> logger
    );

    void loadWallpapers();

    Q_INVOKABLE void installWallpapersFromDialog();

    Q_INVOKABLE void installWallpapersAsync(const QStringList& paths);

    Q_INVOKABLE void applyWallpaperAsync(const QString& wallpaperId) const;

    Q_INVOKABLE void deleteWallpaperAsync(const QString& wallpaperId) const;

    Q_INVOKABLE void refreshWallpapers();

    Q_INVOKABLE auto rowCount(const QModelIndex& parent) const -> int override;

    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const -> QVariant override;

    auto roleNames() const -> QHash<int, QByteArray> override;

private:
    sptr<WallpaperLoaderManager> wallpaperLoader;
    sptr<WallpaperRepository> wallpaperRepository;
    sptr<Config> config;
    sptr<PreviewManager> previewManager;
    sptr<util::Logger> logger;

    void installWallpapers(const QStringList& paths);
};
