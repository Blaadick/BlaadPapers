// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

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

    auto getOptions() const noexcept -> const std::unordered_map<std::string, uptr<Option>>&;

    void addHandler(std::string domain, uptr<DeeplinkHandler> handler) noexcept;

    void addOption(std::string, uptr<Option> option, const std::unordered_set<sptr<Flag>>& flags = {}) noexcept;

    auto execute(int argc, char* argv[]) -> int;

private:
    std::unordered_map<std::string, uptr<DeeplinkHandler>> deeplinkHandlers;
    std::unordered_map<std::string, uptr<Option>> options;
    sptr<util::Logger> logger;
};
