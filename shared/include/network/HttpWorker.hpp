// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <optional>
#include <string>

class HttpWorker {
public:
    HttpWorker();

    ~HttpWorker();

    std::optional<std::string> getString(std::string_view url) const;
};
