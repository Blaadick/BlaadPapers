// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

#include "util/Trash.hpp"

#ifdef __linux__
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <fcntl.h>
#include <format>
#include <optional>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include "network/Uri.hpp"
#include "util/PathUtils.hpp"
#elif _WIN32
#include <windows.h>
#include <shobjidl.h>
#endif

namespace fs = std::filesystem;

#ifdef __linux__
namespace {
    auto deviceOf(const fs::path& path) -> std::optional<dev_t> {
        struct stat pathStat{};
        if(lstat(path.c_str(), &pathStat) != 0) {
            return std::nullopt;
        }

        return pathStat.st_dev;
    }

    auto mountPointOf(fs::path path, const dev_t device) -> fs::path {
        while(path.has_relative_path()) {
            auto parentPath = path.parent_path();
            if(deviceOf(parentPath) != device) {
                break;
            }

            path = std::move(parentPath);
        }

        return path;
    }

    auto isOwnedDir(const fs::path& path) -> bool {
        struct stat pathStat{};
        return lstat(path.c_str(), &pathStat) == 0 && S_ISDIR(pathStat.st_mode) && pathStat.st_uid == getuid();
    }

    auto createTrashDir(const fs::path& trashDirPath) -> bool {
        for(const auto& dirPath : {trashDirPath, trashDirPath / "files", trashDirPath / "info"}) {
            if(mkdir(dirPath.c_str(), 0700) != 0 && errno != EEXIST) {
                return false;
            }

            if(!isOwnedDir(dirPath)) {
                return false;
            }
        }

        return true;
    }

    auto homeTrashDir() -> fs::path {
        const auto dataHomeEnv = std::getenv("XDG_DATA_HOME");
        const auto dataHomeDir = dataHomeEnv && *dataHomeEnv ? fs::path(dataHomeEnv) : util::userHomeDir() / ".local" / "share";
        return dataHomeDir / "Trash";
    }

    auto topDirTrashDir(const fs::path& topDirPath) -> std::optional<fs::path> {
        const auto uid = std::to_string(getuid());
        const auto adminTrashDirPath = topDirPath / ".Trash";

        struct stat adminTrashStat{};
        if(
            lstat(adminTrashDirPath.c_str(), &adminTrashStat) == 0
            && S_ISDIR(adminTrashStat.st_mode)
            && adminTrashStat.st_mode & S_ISVTX
            && createTrashDir(adminTrashDirPath / uid)
        ) {
            return adminTrashDirPath / uid;
        }

        const auto userTrashDirPath = topDirPath / std::format(".Trash-{}", uid);
        if(createTrashDir(userTrashDirPath)) {
            return userTrashDirPath;
        }

        return std::nullopt;
    }

    auto encodeTrashInfoPath(const fs::path& path) -> std::string {
        std::string result = path.is_absolute() ? "/" : "";

        for(const auto& part : path.relative_path()) {
            if(!result.empty() && result.back() != '/') {
                result += '/';
            }

            result += percentEncode(part.string());
        }

        return result;
    }
}

auto util::moveToTrash(const fs::path& path) -> bool {
    const auto absolutePath = fs::absolute(path).lexically_normal();
    const auto pathDevice = deviceOf(absolutePath);
    if(!pathDevice.has_value()) {
        return false;
    }

    const auto homeTrashDirPath = homeTrashDir();
    std::error_code errorCode;
    fs::create_directories(homeTrashDirPath.parent_path(), errorCode);

    fs::path trashDirPath;
    fs::path trashInfoPath;

    if(deviceOf(homeTrashDirPath.parent_path()) == pathDevice && createTrashDir(homeTrashDirPath)) {
        trashDirPath = homeTrashDirPath;
        trashInfoPath = absolutePath;
    } else {
        const auto topDirPath = mountPointOf(absolutePath, *pathDevice);
        const auto topDirTrashDirPath = topDirTrashDir(topDirPath);
        if(!topDirTrashDirPath.has_value()) {
            return false;
        }

        trashDirPath = *topDirTrashDirPath;
        trashInfoPath = absolutePath.lexically_relative(topDirPath);
    }

    const auto deletionDate = std::chrono::floor<std::chrono::seconds>(
        std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time()
    );
    const auto trashInfo = std::format(
        "[Trash Info]\nPath={}\nDeletionDate={:%FT%T}\n",
        encodeTrashInfoPath(trashInfoPath),
        deletionDate
    );

    const auto baseName = absolutePath.filename().string();

    for(int i = 1;; ++i) {
        const auto trashName = i == 1 ? baseName : std::format("{}.{}", baseName, i);
        const auto trashedFilePath = trashDirPath / "files" / trashName;
        const auto infoFilePath = trashDirPath / "info" / (trashName + ".trashinfo");

        if(fs::exists(fs::symlink_status(trashedFilePath, errorCode))) {
            continue;
        }

        const auto infoFile = open(infoFilePath.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0600);
        if(infoFile < 0) {
            if(errno == EEXIST) {
                continue;
            }

            return false;
        }

        const auto written = write(infoFile, trashInfo.data(), trashInfo.size());
        const auto closed = close(infoFile) == 0;

        if(written != static_cast<ssize_t>(trashInfo.size()) || !closed || std::rename(absolutePath.c_str(), trashedFilePath.c_str()) != 0) {
            unlink(infoFilePath.c_str());
            return false;
        }

        return true;
    }
}
#elif _WIN32
auto util::moveToTrash(const fs::path& path) -> bool {
    const auto comInitResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    IFileOperation* fileOperation = nullptr;
    IShellItem* item = nullptr;
    auto result = false;

    if(
        SUCCEEDED(CoCreateInstance(CLSID_FileOperation, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&fileOperation)))
        && SUCCEEDED(fileOperation->SetOperationFlags(FOF_ALLOWUNDO | FOFX_RECYCLEONDELETE | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT))
        && SUCCEEDED(SHCreateItemFromParsingName(fs::absolute(path).lexically_normal().c_str(), nullptr, IID_PPV_ARGS(&item)))
        && SUCCEEDED(fileOperation->DeleteItem(item, nullptr))
        && SUCCEEDED(fileOperation->PerformOperations())
    ) {
        BOOL aborted = FALSE;
        result = SUCCEEDED(fileOperation->GetAnyOperationsAborted(&aborted)) && !aborted;
    }

    if(item) {
        item->Release();
    }

    if(fileOperation) {
        fileOperation->Release();
    }

    if(SUCCEEDED(comInitResult)) {
        CoUninitialize();
    }

    return result;
}
#endif
