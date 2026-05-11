// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/RunRendererOption.hpp"

RunRendererOption::RunRendererOption() : Option("run-renderer", "Runs the renderer daemon") {}

int RunRendererOption::execute(const std::unordered_set<sptr<Argument>>& arguments, const std::vector<std::string>& parameters) {
    return 0;
}
