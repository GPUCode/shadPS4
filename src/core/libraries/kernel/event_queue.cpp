// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/assert.h"
#include "common/debug.h"
#include "core/libraries/kernel/event_queue.h"

namespace Libraries::Kernel {

EqueueInternal::~EqueueInternal() = default;

int EqueueInternal::addEvent(const EqueueEvent& event) {
    std::scoped_lock lock{m_mutex};

    ASSERT(m_events.empty());
    ASSERT(!event.isTriggered);

    // TODO check if event is already exists and return it. Currently we just add in m_events array
    m_events.push_back(event);
    return 0;
}

int EqueueInternal::waitForEvents(SceKernelEvent* ev, int num, u32 micros) {
    std::unique_lock lock{m_mutex};
    int ret = 0;

    const auto predicate = [&] {
        ret = getTriggeredEvents(ev, num);
        return ret > 0;
    };
#ifndef _WIN64
    char buf[128];
    pthread_getname_np(pthread_self(), buf, 128);
    fmt::print("Thread {} waiting for events (micros = {})\n", buf, micros);
#endif // !_WIN64
    if (micros == 0) {
        m_cond.wait(lock, predicate);
    } else {
        m_cond.wait_for(lock, std::chrono::microseconds(micros), predicate);
    }
    fmt::print("Wait done\n");
    return ret;
}

bool EqueueInternal::triggerEvent(u64 ident, s16 filter, void* trigger_data) {
    std::scoped_lock lock{m_mutex};

    ASSERT(m_events.size() <= 1);

    auto& event = m_events[0];
    event.trigger(trigger_data);
    m_cond.notify_one();

    return true;
}

int EqueueInternal::getTriggeredEvents(SceKernelEvent* ev, int num) {
    int ret = 0;

    ASSERT(m_events.size() <= 1);
    auto& event = m_events[0];

    if (event.isTriggered) {
        ev[ret++] = event.event;
        event.reset();
    }

    return ret;
}

} // namespace Libraries::Kernel
