// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "deeplink_handler/DeeplinkHandler.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"
#include "parameter/Parameter.hpp"
#include "util/Pointers.hpp"

class CliExecutor {
public:
    explicit CliExecutor(sptr<util::Logger> logger);

    void addHandler(std::string domain, uptr<DeeplinkHandler> handler);

    void addOption(std::string, uptr<Option> option, std::unordered_set<sptr<Parameter>> parameters = {});

    int execute(int argc, const char** argv);

private:
    std::unordered_map<std::string, uptr<DeeplinkHandler>> deeplinkHandlers;
    std::unordered_map<std::string, uptr<Option>> options;
    std::unordered_set<sptr<Parameter>> allParameters;
    sptr<util::Logger> logger;
};
