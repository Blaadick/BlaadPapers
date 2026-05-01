// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <unordered_map>
#include "OldOption.hpp"

class OldOptionExecutor {
public:
    static void execute(int argc, char** argv);

private:
    static std::unordered_map<char, OldOption> options;
};
