// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
using sptr = std::shared_ptr<T>;

template<class T>
using wptr = std::weak_ptr<T>;
