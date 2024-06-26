// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/assert.h"
#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/kernel/event_queues.h"

namespace Libraries::Kernel {

int PS4_SYSV_ABI sceKernelCreateEqueue(SceKernelEqueue* eq, const char* name) {
    if (eq == nullptr) {
        LOG_ERROR(Kernel_Event, "Event queue is null!");
        return SCE_KERNEL_ERROR_EINVAL;
    }
    if (name == nullptr) {
        LOG_ERROR(Kernel_Event, "Event queue name is invalid!");
        return SCE_KERNEL_ERROR_EFAULT;
    }
    if (name == NULL) {
        LOG_ERROR(Kernel_Event, "Event queue name is null!");
        return SCE_KERNEL_ERROR_EINVAL;
    }

    // Maximum is 32 including null terminator
    static constexpr size_t MaxEventQueueNameSize = 32;
    if (std::strlen(name) > MaxEventQueueNameSize) {
        LOG_ERROR(Kernel_Event, "Event queue name exceeds 32 bytes!");
        return SCE_KERNEL_ERROR_ENAMETOOLONG;
    }

    LOG_INFO(Kernel_Event, "name = {}", name);

    *eq = new EqueueInternal;
    (*eq)->setName(std::string(name));
    return SCE_OK;
}

int PS4_SYSV_ABI sceKernelWaitEqueue(SceKernelEqueue eq, SceKernelEvent* ev, int num, int* out,
                                     SceKernelUseconds* timo) {
    LOG_INFO(Kernel_Event, "num = {}", num);

    if (eq == nullptr) {
        return SCE_KERNEL_ERROR_EBADF;
    }

    if (ev == nullptr) {
        return SCE_KERNEL_ERROR_EFAULT;
    }

    if (num < 1) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    if (timo == nullptr) { // wait until an event arrives without timing out
        *out = eq->waitForEvents(ev, num, 0);
    }

    if (timo != nullptr) {
        // Only events that have already arrived at the time of this function call can be received
        if (*timo == 0) {
            UNREACHABLE();
        } else {
            // Wait until an event arrives with timing out
            UNREACHABLE();
        }
    }

    return SCE_OK;
}

} // namespace Libraries::Kernel
