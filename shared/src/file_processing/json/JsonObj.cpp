// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "file_processing/json/JsonObj.hpp"

#include "file_processing/json/JsonArr.hpp"
#include "file_processing/json/JsonError.hpp"

JsonObj::JsonObj(sptr<JsonDocHolder> doc, yyjson_val* root) : doc(std::move(doc)), root(root) {}

auto JsonObj::tryParse(const std::filesystem::path& filePath) noexcept -> std::expected<JsonObj, std::string> {
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

auto JsonObj::parse(const std::filesystem::path& filePath) -> JsonObj {
    auto jsonObj = tryParse(filePath);
    if(!jsonObj.has_value()) {
        throw JsonError(jsonObj.error());
    }

    return *jsonObj;
}

auto JsonObj::tryGetObj(std::string_view key) const noexcept -> std::expected<JsonObj, std::string> {
    auto objData = yyjson_obj_getn(root, key.data(), key.size());
    if(!objData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_obj(objData)) {
        return std::unexpected("Value is not an object");
    }

    return JsonObj(doc, objData);
}

auto JsonObj::getObj(std::string_view key) const -> JsonObj {
    auto objectVal = tryGetObj(std::move(key));
    if(!objectVal.has_value()) {
        throw JsonError(objectVal.error());
    }

    return *objectVal;
}

auto JsonObj::tryGetArr(std::string_view key) const noexcept -> std::expected<JsonArr, std::string> {
    auto arrayData = yyjson_obj_getn(root, key.data(), key.size());
    if(!arrayData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_arr(arrayData)) {
        return std::unexpected("Value is not an array");
    }

    return JsonArr(doc, arrayData);
}

auto JsonObj::getArr(std::string_view key) const -> JsonArr {
    auto arrayVal = tryGetArr(std::move(key));
    if(!arrayVal.has_value()) {
        throw JsonError(arrayVal.error());
    }

    return *arrayVal;
}

auto JsonObj::tryGetString(std::string_view key) const noexcept -> std::expected<std::string_view, std::string> {
    auto stringData = yyjson_obj_getn(root, key.data(), key.size());
    if(!stringData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_str(stringData)) {
        return std::unexpected("Value is not a string");
    }

    return unsafe_yyjson_get_str(stringData);
}

auto JsonObj::getString(std::string_view key) const -> std::string_view {
    auto stringVal = tryGetString(std::move(key));
    if(!stringVal.has_value()) {
        throw JsonError(stringVal.error());
    }

    return *stringVal;
}

auto JsonObj::tryGetBool(std::string_view key) const noexcept -> std::expected<bool, std::string> {
    auto boolData = yyjson_obj_getn(root, key.data(), key.size());
    if(!boolData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_bool(boolData)) {
        return std::unexpected("Value is not a boolean");
    }

    return unsafe_yyjson_get_bool(boolData);
}

auto JsonObj::getBool(std::string_view key) const -> bool {
    auto boolVal = tryGetBool(std::move(key));
    if(!boolVal.has_value()) {
        throw JsonError(boolVal.error());
    }

    return *boolVal;
}

auto JsonObj::tryGetInt(std::string_view key) const noexcept -> std::expected<int, std::string> {
    auto intData = yyjson_obj_getn(root, key.data(), key.size());
    if(!intData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_int(intData)) {
        return std::unexpected("Value is not an integer");
    }

    return unsafe_yyjson_get_int(intData);
}

auto JsonObj::getInt(std::string_view key) const -> int {
    auto intVal = tryGetInt(std::move(key));
    if(!intVal.has_value()) {
        throw JsonError(intVal.error());
    }

    return *intVal;
}

auto JsonObj::tryGetDouble(std::string_view key) const noexcept -> std::expected<double, std::string> {
    auto doubleData = yyjson_obj_getn(root, key.data(), key.size());
    if(!doubleData) {
        return std::unexpected("No such value");
    }

    if(!yyjson_is_real(doubleData)) {
        return std::unexpected("Value is not a double");
    }

    return unsafe_yyjson_get_real(doubleData);
}

auto JsonObj::getDouble(std::string_view key) const -> double {
    auto doubleVal = tryGetDouble(std::move(key));
    if(!doubleVal.has_value()) {
        throw JsonError(doubleVal.error());
    }

    return *doubleVal;
}
