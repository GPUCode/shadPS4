// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <fstream>
#include <string>
#include <fmt/core.h>
#include <toml.hpp>
#include "config.h"

namespace Config {

bool isNeo = false;
u32 screenWidth = 1280;
u32 screenHeight = 720;
s32 gpuId = -1; // Vulkan physical device index. Set to negative for auto select
std::string logFilter;
std::string logType = "sync";
bool isDebugDump = false;
bool isLibc = true;

bool isLleLibc() {
    return isLibc;
}
bool isNeoMode() {
    return isNeo;
}

u32 getScreenWidth() {
    return screenWidth;
}

u32 getScreenHeight() {
    return screenHeight;
}

s32 getGpuId() {
    return gpuId;
}

std::string getLogFilter() {
    return logFilter;
}

std::string getLogType() {
    return logType;
}

bool debugDump() {
    return isDebugDump;
}

void load(const std::filesystem::path& path) {
    // If the configuration file does not exist, create it and return
    std::error_code error;
    if (!std::filesystem::exists(path, error)) {
        save(path);
        return;
    }

    toml::value data;

    try {
        data = toml::parse(path);
    } catch (std::exception& ex) {
        fmt::print("Got exception trying to load config file. Exception: {}\n", ex.what());
        return;
    }

    if (data.contains("General")) {
        auto generalResult = toml::expect<toml::value>(data.at("General"));
        if (generalResult.is_ok()) {
            auto general = generalResult.unwrap();

            isNeo = toml::find_or<toml::boolean>(general, "isPS4Pro", false);
            logFilter = toml::find_or<toml::string>(general, "logFilter", "");
            logType = toml::find_or<toml::string>(general, "logType", "sync");
        }
    }
    if (data.contains("GPU")) {
        auto gpuResult = toml::expect<toml::value>(data.at("GPU"));
        if (gpuResult.is_ok()) {
            auto gpu = gpuResult.unwrap();

            screenWidth = toml::find_or<toml::integer>(gpu, "screenWidth", screenWidth);
            screenHeight = toml::find_or<toml::integer>(gpu, "screenHeight", screenHeight);
            gpuId = toml::find_or<toml::integer>(gpu, "gpuId", 0);
        }
    }
    if (data.contains("Debug")) {
        auto debugResult = toml::expect<toml::value>(data.at("Debug"));
        if (debugResult.is_ok()) {
            auto debug = debugResult.unwrap();

            isDebugDump = toml::find_or<toml::boolean>(debug, "DebugDump", false);
        }
    }
    if (data.contains("LLE")) {
        auto lleResult = toml::expect<toml::value>(data.at("LLE"));
        if (lleResult.is_ok()) {
            auto lle = lleResult.unwrap();

            isLibc = toml::find_or<toml::boolean>(lle, "libc", true);
        }
    }
}
void save(const std::filesystem::path& path) {
    toml::basic_value<toml::preserve_comments> data;

    std::error_code error;
    if (std::filesystem::exists(path, error)) {
        try {
            data = toml::parse<toml::preserve_comments>(path);
        } catch (const std::exception& ex) {
            fmt::print("Exception trying to parse config file. Exception: {}\n", ex.what());
            return;
        }
    } else {
        if (error) {
            fmt::print("Filesystem error accessing {} (error: {})\n", path.string(),
                       error.message().c_str());
        }
        fmt::print("Saving new configuration file {}\n", path.string());
    }

    data["General"]["isPS4Pro"] = isNeo;
    data["General"]["logFilter"] = logFilter;
    data["General"]["logType"] = logType;
    data["GPU"]["gpuId"] = gpuId;
    data["GPU"]["screenWidth"] = screenWidth;
    data["GPU"]["screenHeight"] = screenHeight;
    data["Debug"]["DebugDump"] = isDebugDump;
    data["LLE"]["libc"] = isLibc;

    std::ofstream file(path, std::ios::out);
    file << data;
    file.close();
}
} // namespace Config
