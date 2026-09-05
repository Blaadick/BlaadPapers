// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <yyjson.h>

class JsonDocHolder final {
public:
    JsonDocHolder(yyjson_doc* doc);

    JsonDocHolder(const JsonDocHolder&) = delete;

    ~JsonDocHolder();

    JsonDocHolder& operator=(const JsonDocHolder&) = delete;

    operator yyjson_doc*() const;

private:
    yyjson_doc* doc;
};
