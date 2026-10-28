// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SecretStorage.hpp"

#include <format>

#ifdef _WIN32
#include <system_error>
#include <windows.h>
#include <wincred.h>
#else
#include <libsecret/secret.h>
#endif

#ifdef _WIN32
auto SecretStorage::get(const std::string& name) const -> std::expected<std::string, std::string> {
    PCREDENTIALA credential = nullptr;
    if(!CredReadA(std::format("{}/{}", PROJECT_NAME, name).c_str(), CRED_TYPE_GENERIC, 0, &credential)) {
        if(GetLastError() == ERROR_NOT_FOUND) {
            return std::unexpected(std::format("Secret \"{}\" not found", name));
        }

        return std::unexpected(std::system_category().message(static_cast<int>(GetLastError())));
    }

    std::string secret(reinterpret_cast<const char*>(credential->CredentialBlob), credential->CredentialBlobSize);
    CredFree(credential);

    if(secret.empty()) {
        return std::unexpected(std::format("Secret \"{}\" is empty", name));
    }

    return secret;
}

auto SecretStorage::set(const std::string& name, const std::string& value) const -> std::expected<void, std::string> {
    auto target = std::format("{}/{}", PROJECT_NAME, name);

    CREDENTIALA credential = {};
    credential.Type = CRED_TYPE_GENERIC;
    credential.TargetName = target.data();
    credential.CredentialBlobSize = static_cast<DWORD>(value.size());
    credential.CredentialBlob = reinterpret_cast<LPBYTE>(const_cast<char*>(value.data()));
    credential.Persist = CRED_PERSIST_LOCAL_MACHINE;

    if(!CredWriteA(&credential, 0)) {
        return std::unexpected(std::system_category().message(static_cast<int>(GetLastError())));
    }

    return {};
}
#else
static constexpr SecretSchema schema = {
    .name = "blaadpapers",
    .flags = SECRET_SCHEMA_NONE,
    .attributes = {
        {"name", SECRET_SCHEMA_ATTRIBUTE_STRING},
        {nullptr, SECRET_SCHEMA_ATTRIBUTE_STRING}
    }
};

auto SecretStorage::get(const std::string& name) const -> std::expected<std::string, std::string> {
    GError* error = nullptr;
    auto password = secret_password_lookup_sync(&schema, nullptr, &error, "name", name.c_str(), nullptr);

    if(error != nullptr) {
        std::string message = error->message;
        g_error_free(error);
        return std::unexpected(message);
    }

    if(password == nullptr) {
        return std::unexpected(std::format("Secret \"{}\" not found", name));
    }

    std::string secret = password;
    secret_password_free(password);

    if(secret.empty()) {
        return std::unexpected(std::format("Secret \"{}\" is empty", name));
    }

    return secret;
}

auto SecretStorage::set(const std::string& name, const std::string& value) const -> std::expected<void, std::string> {
    GError* error = nullptr;
    auto label = std::format("{} {}", PROJECT_NAME, name);
    secret_password_store_sync(&schema, SECRET_COLLECTION_DEFAULT, label.c_str(), value.c_str(), nullptr, &error, "name", name.c_str(), nullptr);

    if(error != nullptr) {
        std::string message = error->message;
        g_error_free(error);
        return std::unexpected(message);
    }

    return {};
}
#endif
