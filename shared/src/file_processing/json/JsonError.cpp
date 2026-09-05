// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "file_processing/json/JsonError.hpp"

JsonError::JsonError(const std::string& string) : runtime_error(string) {}

JsonError::JsonError(const char* string) : runtime_error(string) {}
