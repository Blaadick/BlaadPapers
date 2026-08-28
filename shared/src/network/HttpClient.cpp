// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "network/HttpClient.hpp"

#include <fstream>
#include <curl/curl.h>
#include "util/PathUtils.hpp"

namespace fs = std::filesystem;

namespace {
    size_t writeToString(const char* data, const size_t size, const size_t count, void* userdata) {
        auto* buf = static_cast<std::string*>(userdata);
        buf->append(data, size * count);

        return size * count;
    }

    size_t writeToFile(const char* data, const size_t size, const size_t count, void* userdata) {
        auto& file = *static_cast<std::ofstream*>(userdata);
        auto bytes = size * count;

        file.write(data, static_cast<std::streamsize>(bytes));

        return file ? bytes : 0;
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
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, PROJECT_USER_AGENT);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
    auto result = curl_easy_perform(curl);

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if(result != CURLE_OK || httpCode != 200) {
        return std::nullopt;
    }

    return output;
}

std::optional<std::string> HttpClient::requestContentType(const std::string_view url) const {
    std::optional<std::string> response;

    auto curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, PROJECT_USER_AGENT);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    auto result = curl_easy_perform(curl);

    long httpCode = 0;
    char* contentType = nullptr;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);

    if(result == CURLE_OK && httpCode == 200) {
        response = contentType;
    }

    curl_easy_cleanup(curl);
    return response;
}

std::optional<fs::path> HttpClient::downloadFile(
    std::string_view url,
    const fs::path& downloadDir,
    std::optional<std::string_view> fileName
) const {
    util::createDirIfNotExists(downloadDir);

    fs::path finalPath;

    if(fileName.has_value()) {
        finalPath = downloadDir / *fileName;
    } else {

    }

    auto partPath = finalPath += ".part";

    if(fs::exists(partPath)) {
        auto existingPartSize = static_cast<curl_off_t>(fs::file_size(partPath));
        auto file = std::ofstream(partPath, std::ios::binary | std::ios::app);

        auto curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, PROJECT_USER_AGENT);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, existingPartSize);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToFile);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
        auto result = curl_easy_perform(curl);

        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
        curl_easy_cleanup(curl);

        if(result == CURLE_RANGE_ERROR || result == CURLE_BAD_DOWNLOAD_RESUME) {
            fs::remove(partPath);
            return downloadFile(url, downloadDir, fileName);
        }

        if(result != CURLE_OK || httpCode != 200) {
            return std::nullopt;
        }
    } else {
        auto file = std::ofstream(partPath, std::ios::binary | std::ios::trunc);

        auto curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, PROJECT_USER_AGENT);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToFile);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
        auto result = curl_easy_perform(curl);

        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
        curl_easy_cleanup(curl);

        if(result != CURLE_OK || httpCode != 200) {
            return std::nullopt;
        }
    }

    fs::rename(partPath, finalPath);
    return finalPath;
}
