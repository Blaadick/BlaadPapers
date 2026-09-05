// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

#include "file_processing/json/JsonObj.hpp"

#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonError.hpp"

JsonObj::JsonObj(sptr<JsonDocHolder> doc, yyjson_val* root) : doc(std::move(doc)), root(root) {}

std::expected<JsonObj, std::string> JsonObj::tryParse(const std::filesystem::path& filePath) noexcept {
    yyjson_read_err readErr;
    auto doc = yyjson_read_file(filePath.c_str(), YYJSON_READ_NOFLAG, nullptr, &readErr);
    if(!doc) {
        return std::unexpected(readErr.msg);
    }

    auto root = yyjson_doc_get_root(doc);
    if(!yyjson_is_obj(root)) {
        yyjson_doc_free(doc);
        return std::unexpected("Root is not object");
    }

    return JsonObj(std::make_shared<JsonDocHolder>(doc), root);
}

JsonObj JsonObj::parse(const std::filesystem::path& filePath) {
    auto jsonObj = tryParse(filePath);
    if(!jsonObj.has_value()) {
        throw JsonError(jsonObj.error());
    }

    return *jsonObj;
}

std::expected<JsonObj, std::string> JsonObj::tryGetObj(std::string_view key) const noexcept {
    auto objData = yyjson_obj_getn(root, key.data(), key.size());
    if(!objData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_obj(objData)) {
        return std::unexpected("Value is not an object");
    }

    return JsonObj(doc, objData);
}

JsonObj JsonObj::getObj(std::string_view key) const {
    auto objectVal = tryGetObj(std::move(key));
    if(!objectVal.has_value()) {
        throw JsonError(objectVal.error());
    }

    return *objectVal;
}

std::expected<JsonArr, std::string> JsonObj::tryGetArr(std::string_view key) const noexcept {
    auto arrayData = yyjson_obj_getn(root, key.data(), key.size());
    if(!arrayData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_arr(arrayData)) {
        return std::unexpected("Value is not an array");
    }

    return JsonArr(doc, arrayData);
}

JsonArr JsonObj::getArr(std::string_view key) const {
    auto arrayVal = tryGetArr(std::move(key));
    if(!arrayVal.has_value()) {
        throw JsonError(arrayVal.error());
    }

    return *arrayVal;
}

std::expected<std::string_view, std::string> JsonObj::tryGetString(std::string_view key) const noexcept {
    auto stringData = yyjson_obj_getn(root, key.data(), key.size());
    if(!stringData) {
        return std::unexpected("No such value");
    }

    auto stringVal = yyjson_get_str(stringData);
    if(!stringVal) {
        return std::unexpected("Value is not a string");
    }

    return stringVal;
}

std::string_view JsonObj::getString(std::string_view key) const {
    auto stringVal = tryGetString(std::move(key));
    if(!stringVal.has_value()) {
        throw JsonError(stringVal.error());
    }

    return *stringVal;
}

std::expected<bool, std::string> JsonObj::tryGetBool(std::string_view key) const noexcept {
    auto boolData = yyjson_obj_getn(root, key.data(), key.size());
    if(!boolData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_bool(boolData)) {
        return std::unexpected("Value is not a boolean");
    }

    return unsafe_yyjson_get_bool(boolData);
}

bool JsonObj::getBool(std::string_view key) const {
    auto boolVal = tryGetBool(std::move(key));
    if(!boolVal.has_value()) {
        throw JsonError(boolVal.error());
    }

    return *boolVal;
}
