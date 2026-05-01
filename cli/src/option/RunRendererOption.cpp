// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "option/RunRendererOption.hpp"

RunRendererOption::RunRendererOption() : Option("run-renderer", std::nullopt, {}, "Runs the renderer daemon") {}

int RunRendererOption::execute(const std::vector<std::string>& arguments, const std::unordered_set<sptr<SubOption>>& subOptions) {
    return 0;
}
