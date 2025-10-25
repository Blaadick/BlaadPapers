// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QMap>

class Tags {
public:
    [[nodiscard]]
    static const QMap<QString, int>& getUniqueTags();

    [[nodiscard]]
    static int count();

    static void addTag(const QString& newTagName);

    static bool removeTag(const QString& removeTagName);

private:
    static QMap<QString, int> uniqueTags;
};
