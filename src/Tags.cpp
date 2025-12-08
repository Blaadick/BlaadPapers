// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "Tags.hpp"

const QMap<QString, int>& Tags::getUniqueTags() {
    return uniqueTags;
}

int Tags::count() {
    return uniqueTags.count();
}

void Tags::addTag(const QString& newTagName) {
    for(const auto& [name, quantity] : uniqueTags.asKeyValueRange()) {
        if(name == newTagName) {
            ++quantity;
            return;
        }
    }

    uniqueTags[newTagName] = 1;
}

bool Tags::removeTag(const QString& removeTagName) {
    for(const auto& [name, quantity] : uniqueTags.asKeyValueRange()) {
        if(name == removeTagName) {
            quantity > 1 ? --quantity : uniqueTags.remove(name);
            return true;
        }
    }

    return false;
}

QMap<QString, int> Tags::uniqueTags;
