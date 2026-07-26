// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_set>
#include "Flag.hpp"
#include "util/Pointers.hpp"

/**
 * Interim measure.
 */
class Flags {
public:
    static sptr<Flag> help;
    static sptr<Flag> quiet;
    static sptr<Flag> json;

    static const std::unordered_set<sptr<Flag>> all;
};
