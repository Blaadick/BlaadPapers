// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/HttpWorker.hpp"

#include <curl/curl.h>

namespace {
    size_t writeToString(const char* ptr, const size_t size, const size_t nmemb, void* userdata) {
        auto* buf = static_cast<std::string*>(userdata);
        buf->append(ptr, size * nmemb);

        return size * nmemb;
    }
}

HttpWorker::HttpWorker() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpWorker::~HttpWorker() {
    curl_global_cleanup();
}

std::optional<std::string> HttpWorker::getString(const std::string_view url) const {
    std::string string;

    auto curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    auto response = curl_easy_perform(curl);
    if(response != CURLE_OK) {
        return std::nullopt;
    }

    return string;
}
