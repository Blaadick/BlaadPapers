// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_map>

#include "deeplink_handler/DeeplinkHandler.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"
#include "util/Pointers.hpp"

class CliExecutor {
public:
    explicit CliExecutor(sptr<util::Logger> logger);

    void addOption(const std::string&, uptr<Option> option);

    void addHandler(const std::string& domain, uptr<DeeplinkHandler> handler);

    int execute(int argc, const char** argv) const;

private:
    std::unordered_map<std::string, uptr<Option>> options;
    std::unordered_map<std::string, uptr<DeeplinkHandler>> deeplinkHandlers;
    sptr<util::Logger> logger;
};
