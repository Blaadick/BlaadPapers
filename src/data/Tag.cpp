// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "data/Tag.hpp"

Tag::Tag(const QString& name, const int quantity) : name(name), quantity(quantity) {}

const QString& Tag::getName() const {
    return name;
}

int Tag::getQuantity() const {
    return quantity;
}

void Tag::incrementQuantity() {
    ++quantity;
}

void Tag::decrementQuantity() {
    --quantity;

    if(quantity < 1) {
        delete this;
    }
}
