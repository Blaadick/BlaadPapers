// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "flag/Flags.hpp"

sptr<Flag> Flags::help = std::make_shared<Flag>("help", 'h', "Displays help page for option");
sptr<Flag> Flags::quiet = std::make_shared<Flag>("quiet", 'q', "Suppress whole command output");
sptr<Flag> Flags::json = std::make_shared<Flag>("json", 'j', "Outputs command result in JSON format");

const std::unordered_set<sptr<Flag>> Flags::all = {
    help,
    quiet,
    json
};
