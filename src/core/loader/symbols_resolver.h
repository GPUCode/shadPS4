// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "common/types.h"

namespace Core::Loader {

struct SymbolRecord {
    std::string name;
    u64 virtual_address;
};

struct SymbolRes {
    std::string name;
    std::string nidName;
    std::string library;
    u16 library_version;
    std::string module;
    u8 module_version_major;
    u8 module_version_minor;
    u32 type;
};

class SymbolsResolver {
public:
    SymbolsResolver() = default;
    ~SymbolsResolver() = default;

    void AddSymbol(const SymbolRes& s, u64 virtual_addr);
    const SymbolRecord* FindSymbol(const SymbolRes& s) const;

    static std::string GenerateName(const SymbolRes& s);

private:
    std::vector<SymbolRecord> m_symbols;
};

} // namespace Core::Loader
