// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QString>

class Tag {
public:
    explicit Tag(const QString& name, int quantity = 1);

    [[nodiscard]]
    const QString& getName() const;

    [[nodiscard]]
    int getQuantity() const;

    void incrementQuantity();

    void decrementQuantity();

private:
    QString name;
    int quantity;
};
