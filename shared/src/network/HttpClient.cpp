// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/HttpClient.hpp"

#include <curl/curl.h>

namespace {
    size_t writeToString(const char* ptr, const size_t size, const size_t nmemb, void* userdata) {
        auto* buf = static_cast<std::string*>(userdata);
        buf->append(ptr, size * nmemb);

        return size * nmemb;
    }

    size_t writeToBytes(char* ptr, const size_t size, const size_t nmemb, void* userdata) {
        auto* buffer = static_cast<std::vector<std::byte>*>(userdata);
        auto* bytePtr = reinterpret_cast<std::byte*>(ptr);
        buffer->insert(buffer->end(), bytePtr, bytePtr + size * nmemb);
        return size * nmemb;
    }
}

HttpClient::HttpClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpClient::~HttpClient() {
    curl_global_cleanup();
}

std::optional<std::string> HttpClient::requestString(const std::string_view url) const {
    std::string output;

    auto curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, PROJECT_USER_AGENT);
    auto result = curl_easy_perform(curl);

    long httpCode;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if(result != CURLE_OK || httpCode != 200) {
        return std::nullopt;
    }

    return output;
}

std::optional<std::vector<std::byte>> HttpClient::requestBinary(const std::string_view url) const {
    std::vector<std::byte> output;

    auto curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToBytes);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, PROJECT_USER_AGENT);
    auto result = curl_easy_perform(curl);

    long httpCode;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if(result != CURLE_OK || httpCode != 200) {
        return std::nullopt;
    }

    return output;
}
