// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <cstddef>
#include "common/types.h"

namespace Core::Loader {
class SymbolsResolver;
}

namespace Libraries::Net {

void RegisterlibSceNet(Core::Loader::SymbolsResolver* sym);

} // namespace Libraries::Net
