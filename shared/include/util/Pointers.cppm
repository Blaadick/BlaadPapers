// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

export module blaadpapers:Pointers;

import std;

export template<class T>
using uptr = std::unique_ptr<T>;

export template<class T>
using sptr = std::shared_ptr<T>;

export template<class T>
using wptr = std::weak_ptr<T>;
