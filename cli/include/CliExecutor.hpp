// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "deeplink_handler/DeeplinkHandler.hpp"
#include "flag/Flag.hpp"
#include "logger/Logger.hpp"
#include "option/Option.hpp"
#include "util/Pointers.hpp"

class CliExecutor {
public:
    explicit CliExecutor(sptr<util::Logger> logger);

    const std::unordered_map<std::string, uptr<Option>>& getOptions() const;

    void addHandler(std::string domain, uptr<DeeplinkHandler> handler);

    void addOption(std::string, uptr<Option> option, const std::unordered_set<sptr<Flag>>& flags = {});

    int execute(int argc, const char** argv);

private:
    std::unordered_map<std::string, uptr<DeeplinkHandler>> deeplinkHandlers;
    std::unordered_map<std::string, uptr<Option>> options;
    sptr<util::Logger> logger;
};
