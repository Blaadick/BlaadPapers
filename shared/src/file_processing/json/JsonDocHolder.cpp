// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "file_processing/json/JsonDocHolder.hpp"

JsonDocHolder::JsonDocHolder(yyjson_doc* doc) : doc(doc) {}

JsonDocHolder::~JsonDocHolder() {
    yyjson_doc_free(doc);
}

JsonDocHolder::operator yyjson_doc*() const {
    return doc;
}
