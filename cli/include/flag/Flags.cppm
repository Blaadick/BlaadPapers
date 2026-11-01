// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module cli:Flags;

import blaadpapers;
import std;
import :Flag;

/**
 * Interim measure.
 */
export class Flags {
public:
    static sptr<Flag> help;
    static sptr<Flag> quiet;
    static sptr<Flag> json;

    static const std::unordered_set<sptr<Flag>> all;
};
