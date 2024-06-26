// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <mutex>
#include <thread>
#include "common/assert.h"
#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/kernel/thread_management.h"
#include "core/libraries/libs.h"

namespace Libraries::Kernel {

thread_local ScePthread g_pthread_self{};
PThreadCxt* g_pthread_cxt = nullptr;

void init_pthreads() {
    g_pthread_cxt = new PThreadCxt{};
    // default mutex init
    ScePthreadMutexattr default_mutexattr = nullptr;
    scePthreadMutexattrInit(&default_mutexattr);
    g_pthread_cxt->setDefaultMutexattr(default_mutexattr);
    // default cond init
    ScePthreadCondattr default_condattr = nullptr;
    scePthreadCondattrInit(&default_condattr);
    g_pthread_cxt->setDefaultCondattr(default_condattr);
    // default attr init
    ScePthreadAttr default_attr = nullptr;
    scePthreadAttrInit(&default_attr);
    g_pthread_cxt->SetDefaultAttr(default_attr);

    g_pthread_cxt->SetPthreadPool(new PThreadPool);
}

void pthreadInitSelfMainThread() {
    g_pthread_self = new PthreadInternal{};
    scePthreadAttrInit(&g_pthread_self->attr);
    g_pthread_self->pth = pthread_self();
    g_pthread_self->name = "Main_Thread";
}

int PS4_SYSV_ABI scePthreadAttrInit(ScePthreadAttr* attr) {
    *attr = new PthreadAttrInternal{};

    int result = pthread_attr_init(&(*attr)->pth_attr);

    (*attr)->affinity = 0x7f;
    (*attr)->guard_size = 0x1000;

    SceKernelSchedParam param{};
    param.sched_priority = 700;

    result = (result == 0 ? scePthreadAttrSetinheritsched(attr, 4) : result);
    result = (result == 0 ? scePthreadAttrSetschedparam(attr, &param) : result);
    result = (result == 0 ? scePthreadAttrSetschedpolicy(attr, SCHED_OTHER) : result);
    result = (result == 0 ? scePthreadAttrSetdetachstate(attr, PTHREAD_CREATE_JOINABLE) : result);

    switch (result) {
    case 0:
        return SCE_OK;
    case ENOMEM:
        return SCE_KERNEL_ERROR_ENOMEM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

int PS4_SYSV_ABI scePthreadAttrDestroy(ScePthreadAttr* attr) {

    int result = pthread_attr_destroy(&(*attr)->pth_attr);

    delete *attr;
    *attr = nullptr;

    if (result == 0) {
        return SCE_OK;
    }
    return SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrSetguardsize(ScePthreadAttr* attr, size_t guard_size) {
    if (attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    (*attr)->guard_size = guard_size;

    return SCE_OK;
}

int PS4_SYSV_ABI scePthreadAttrGetguardsize(const ScePthreadAttr* attr, size_t* guard_size) {
    if (guard_size == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    *guard_size = (*attr)->guard_size;

    return SCE_OK;
}

int PS4_SYSV_ABI scePthreadAttrGetinheritsched(const ScePthreadAttr* attr, int* inherit_sched) {

    if (inherit_sched == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_attr_getinheritsched(&(*attr)->pth_attr, inherit_sched);

    switch (*inherit_sched) {
    case PTHREAD_EXPLICIT_SCHED:
        *inherit_sched = 0;
        break;
    case PTHREAD_INHERIT_SCHED:
        *inherit_sched = 4;
        break;
    default:
        UNREACHABLE();
    }

    return (result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL);
}

int PS4_SYSV_ABI scePthreadAttrGetdetachstate(const ScePthreadAttr* attr, int* state) {
    if (state == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    // int result = pthread_attr_getdetachstate(&(*attr)->p, state);
    int result = 0;

    *state = ((*attr)->detached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);

    switch (*state) {
    case PTHREAD_CREATE_JOINABLE:
        *state = 0;
        break;
    case PTHREAD_CREATE_DETACHED:
        *state = 1;
        break;
    default:
        UNREACHABLE();
    }

    return (result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL);
}

int PS4_SYSV_ABI scePthreadAttrSetdetachstate(ScePthreadAttr* attr, int detachstate) {
    if (attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int pstate = PTHREAD_CREATE_JOINABLE;
    switch (detachstate) {
    case 0:
        pstate = PTHREAD_CREATE_JOINABLE;
        break;
    case 1:
        pstate = PTHREAD_CREATE_DETACHED;
        break;
    default:
        UNREACHABLE_MSG("Invalid detachstate: {}", detachstate);
    }

    // int result = pthread_attr_setdetachstate(&(*attr)->pth_attr, pstate); doesn't seem to work
    // correctly
    int result = 0;

    (*attr)->detached = (pstate == PTHREAD_CREATE_DETACHED);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrSetinheritsched(ScePthreadAttr* attr, int inheritSched) {
    if (attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int pinherit_sched = PTHREAD_INHERIT_SCHED;
    switch (inheritSched) {
    case 0:
        pinherit_sched = PTHREAD_EXPLICIT_SCHED;
        break;
    case 4:
        pinherit_sched = PTHREAD_INHERIT_SCHED;
        break;
    default:
        UNREACHABLE_MSG("Invalid inheritSched: {}", inheritSched);
    }

    int result = pthread_attr_setinheritsched(&(*attr)->pth_attr, pinherit_sched);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrGetschedparam(const ScePthreadAttr* attr,
                                             SceKernelSchedParam* param) {

    if (param == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_attr_getschedparam(&(*attr)->pth_attr, param);

    if (param->sched_priority <= -2) {
        param->sched_priority = 767;
    } else if (param->sched_priority >= +2) {
        param->sched_priority = 256;
    } else {
        param->sched_priority = 700;
    }

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrSetschedparam(ScePthreadAttr* attr,
                                             const SceKernelSchedParam* param) {
    if (param == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    SceKernelSchedParam pparam{};
    if (param->sched_priority <= 478) {
        pparam.sched_priority = +2;
    } else if (param->sched_priority >= 733) {
        pparam.sched_priority = -2;
    } else {
        pparam.sched_priority = 0;
    }

    int result = pthread_attr_setschedparam(&(*attr)->pth_attr, &pparam);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrGetschedpolicy(const ScePthreadAttr* attr, int* policy) {

    if (policy == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_attr_getschedpolicy(&(*attr)->pth_attr, policy);

    switch (*policy) {
    case SCHED_OTHER:
        *policy = (*attr)->policy;
        break;
    case SCHED_FIFO:
        *policy = 1;
        break;
    case SCHED_RR:
        *policy = 3;
        break;
    default:
        UNREACHABLE();
    }

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrSetschedpolicy(ScePthreadAttr* attr, int policy) {
    if (attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int ppolicy = SCHED_OTHER; // winpthreads only supports SCHED_OTHER
    if (policy != SCHED_OTHER) {
        LOG_ERROR(Kernel_Pthread, "policy={} not supported by winpthreads\n", policy);
    }
    (*attr)->policy = policy;

    int result = pthread_attr_setschedpolicy(&(*attr)->pth_attr, ppolicy);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}
ScePthread PS4_SYSV_ABI scePthreadSelf() {
    return g_pthread_self;
}

int PS4_SYSV_ABI scePthreadAttrSetaffinity(ScePthreadAttr* pattr,
                                           const /*SceKernelCpumask*/ u64 mask) {
    LOG_INFO(Kernel_Pthread, "called");

    if (pattr == nullptr || *pattr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    (*pattr)->affinity = mask;

    return SCE_OK;
}

int PS4_SYSV_ABI scePthreadAttrGetaffinity(const ScePthreadAttr* pattr,
                                           /* SceKernelCpumask*/ u64* mask) {
    if (pattr == nullptr || *pattr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    *mask = (*pattr)->affinity;

    return SCE_OK;
}

int PS4_SYSV_ABI scePthreadAttrGetstackaddr(const ScePthreadAttr* attr, void** stack_addr) {

    if (stack_addr == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_attr_getstackaddr(&(*attr)->pth_attr, stack_addr);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrGetstacksize(const ScePthreadAttr* attr, size_t* stack_size) {

    if (stack_size == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_attr_getstacksize(&(*attr)->pth_attr, stack_size);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrSetstackaddr(ScePthreadAttr* attr, void* addr) {

    if (addr == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_attr_setstackaddr(&(*attr)->pth_attr, addr);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadAttrSetstacksize(ScePthreadAttr* attr, size_t stack_size) {

    if (stack_size == 0 || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_attr_setstacksize(&(*attr)->pth_attr, stack_size);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadSetaffinity(ScePthread thread, const /*SceKernelCpumask*/ u64 mask) {
    LOG_INFO(Kernel_Pthread, "called");

    if (thread == nullptr) {
        return SCE_KERNEL_ERROR_ESRCH;
    }

    auto result = scePthreadAttrSetaffinity(&thread->attr, mask);

    return result;
}

void* createMutex(void* addr) {
    if (addr == nullptr || *static_cast<ScePthreadMutex*>(addr) != nullptr) {
        return addr;
    }
    auto vaddr = reinterpret_cast<u64>(addr);

    std::string name = fmt::format("mutex{:#x}", vaddr);
    scePthreadMutexInit(static_cast<ScePthreadMutex*>(addr), nullptr, name.c_str());
    return addr;
}

int PS4_SYSV_ABI scePthreadMutexInit(ScePthreadMutex* mutex, const ScePthreadMutexattr* attr,
                                     const char* name) {
    if (mutex == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }
    if (attr == nullptr) {
        attr = g_pthread_cxt->getDefaultMutexattr();
    }

    *mutex = new PthreadMutexInternal{};
    if (name != nullptr) {
        (*mutex)->name = name;
    } else {
        (*mutex)->name = "nonameMutex";
    }

    int result = pthread_mutex_init(&(*mutex)->pth_mutex, &(*attr)->pth_mutex_attr);

    if (name != nullptr) {
        LOG_INFO(Kernel_Pthread, "name={}, result={}", name, result);
    }

    switch (result) {
    case 0:
        return SCE_OK;
    case EAGAIN:
        return SCE_KERNEL_ERROR_EAGAIN;
    case EINVAL:
        return SCE_KERNEL_ERROR_EINVAL;
    case ENOMEM:
        return SCE_KERNEL_ERROR_ENOMEM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

int PS4_SYSV_ABI scePthreadMutexDestroy(ScePthreadMutex* mutex) {

    if (mutex == nullptr || *mutex == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_mutex_destroy(&(*mutex)->pth_mutex);

    LOG_INFO(Kernel_Pthread, "name={}, result={}", (*mutex)->name, result);

    delete *mutex;
    *mutex = nullptr;

    switch (result) {
    case 0:
        return SCE_OK;
    case EBUSY:
        return SCE_KERNEL_ERROR_EBUSY;
    case EINVAL:
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}
int PS4_SYSV_ABI scePthreadMutexattrInit(ScePthreadMutexattr* attr) {
    *attr = new PthreadMutexattrInternal{};

    int result = pthread_mutexattr_init(&(*attr)->pth_mutex_attr);

    result = (result == 0 ? scePthreadMutexattrSettype(attr, 1) : result);
    result = (result == 0 ? scePthreadMutexattrSetprotocol(attr, 0) : result);

    switch (result) {
    case 0:
        return SCE_OK;
    case ENOMEM:
        return SCE_KERNEL_ERROR_ENOMEM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

int PS4_SYSV_ABI scePthreadMutexattrSettype(ScePthreadMutexattr* attr, int type) {
    int ptype = PTHREAD_MUTEX_DEFAULT;
    switch (type) {
    case 1:
        ptype = PTHREAD_MUTEX_ERRORCHECK;
        break;
    case 2:
        ptype = PTHREAD_MUTEX_RECURSIVE;
        break;
    case 3:
    case 4:
        ptype = PTHREAD_MUTEX_NORMAL;
        break;
    default:
        UNREACHABLE_MSG("Invalid type: {}", type);
    }

    int result = pthread_mutexattr_settype(&(*attr)->pth_mutex_attr, ptype);

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI scePthreadMutexattrSetprotocol(ScePthreadMutexattr* attr, int protocol) {
    int pprotocol = PTHREAD_PRIO_NONE;
    switch (protocol) {
    case 0:
        pprotocol = PTHREAD_PRIO_NONE;
        break;
    case 1:
        pprotocol = PTHREAD_PRIO_INHERIT;
        break;
    case 2:
        pprotocol = PTHREAD_PRIO_PROTECT;
        break;
    default:
        UNREACHABLE_MSG("Invalid protocol: {}", protocol);
    }

    int result = 0; // pthread_mutexattr_setprotocol(&(*attr)->p, pprotocol); //it appears that
                    // pprotocol has issues in winpthreads
    (*attr)->pprotocol = pprotocol;

    return result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL;
}
int PS4_SYSV_ABI scePthreadMutexLock(ScePthreadMutex* mutex) {
    mutex = static_cast<ScePthreadMutex*>(createMutex(mutex));

    if (mutex == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_mutex_lock(&(*mutex)->pth_mutex);
    if (result != 0) {
        LOG_INFO(Kernel_Pthread, "name={}, result={}", (*mutex)->name, result);
    }
    switch (result) {
    case 0:
        return SCE_OK;
    case EAGAIN:
        return SCE_KERNEL_ERROR_EAGAIN;
    case EINVAL:
        return SCE_KERNEL_ERROR_EINVAL;
    case EDEADLK:
        return SCE_KERNEL_ERROR_EDEADLK;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}
int PS4_SYSV_ABI scePthreadMutexUnlock(ScePthreadMutex* mutex) {
    mutex = static_cast<ScePthreadMutex*>(createMutex(mutex));
    if (mutex == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_mutex_unlock(&(*mutex)->pth_mutex);
    if (result != 0) {
        LOG_INFO(Kernel_Pthread, "name={}, result={}", (*mutex)->name, result);
    }
    switch (result) {
    case 0:
        return SCE_OK;

    case EINVAL:
        return SCE_KERNEL_ERROR_EINVAL;
    case EPERM:
        return SCE_KERNEL_ERROR_EPERM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

int PS4_SYSV_ABI scePthreadMutexattrDestroy(ScePthreadMutexattr* attr) {

    int result = pthread_mutexattr_destroy(&(*attr)->pth_mutex_attr);

    delete *attr;
    *attr = nullptr;

    switch (result) {
    case 0:
        return SCE_OK;
    case ENOMEM:
        return SCE_KERNEL_ERROR_ENOMEM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

void* createCond(void* addr) {
    if (addr == nullptr || *static_cast<ScePthreadCond*>(addr) != nullptr) {
        return addr;
    }
    auto vaddr = reinterpret_cast<u64>(addr);

    std::string name = fmt::format("cond{:#x}", vaddr);
    scePthreadCondInit(static_cast<ScePthreadCond*>(addr), nullptr, name.c_str());
    return addr;
}

int PS4_SYSV_ABI scePthreadCondInit(ScePthreadCond* cond, const ScePthreadCondattr* attr,
                                    const char* name) {
    if (cond == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    if (attr == nullptr) {
        attr = g_pthread_cxt->getDefaultCondattr();
    }

    *cond = new PthreadCondInternal{};

    if (name != nullptr) {
        (*cond)->name = name;
    } else {
        (*cond)->name = "nonameCond";
    }

    int result = pthread_cond_init(&(*cond)->cond, &(*attr)->cond_attr);

    if (name != nullptr) {
        LOG_INFO(Kernel_Pthread, "name={}, result={}", (*cond)->name, result);
    }

    switch (result) {
    case 0:
        return SCE_OK;
    case EAGAIN:
        return SCE_KERNEL_ERROR_EAGAIN;
    case EINVAL:
        return SCE_KERNEL_ERROR_EINVAL;
    case ENOMEM:
        return SCE_KERNEL_ERROR_ENOMEM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

int PS4_SYSV_ABI scePthreadCondattrInit(ScePthreadCondattr* attr) {
    *attr = new PthreadCondAttrInternal{};

    int result = pthread_condattr_init(&(*attr)->cond_attr);

    switch (result) {
    case 0:
        return SCE_OK;
    case ENOMEM:
        return SCE_KERNEL_ERROR_ENOMEM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

int PS4_SYSV_ABI scePthreadCondBroadcast(ScePthreadCond* cond) {
    LOG_INFO(Kernel_Pthread, "called");
    cond = static_cast<ScePthreadCond*>(createCond(cond));

    if (cond == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    int result = pthread_cond_broadcast(&(*cond)->cond);

    LOG_INFO(Kernel_Pthread, "name={}, result={}", (*cond)->name, result);

    return (result == 0 ? SCE_OK : SCE_KERNEL_ERROR_EINVAL);
}

int PS4_SYSV_ABI posix_pthread_mutex_init(ScePthreadMutex* mutex, const ScePthreadMutexattr* attr) {
    // LOG_INFO(Kernel_Pthread, "posix pthread_mutex_init redirect to scePthreadMutexInit");
    int result = scePthreadMutexInit(mutex, attr, nullptr);
    if (result < 0) {
        int rt = result > SCE_KERNEL_ERROR_UNKNOWN && result <= SCE_KERNEL_ERROR_ESTOP
                     ? result + -SCE_KERNEL_ERROR_UNKNOWN
                     : POSIX_EOTHER;
        return rt;
    }
    return result;
}

int PS4_SYSV_ABI posix_pthread_mutex_lock(ScePthreadMutex* mutex) {
    // LOG_INFO(Kernel_Pthread, "posix pthread_mutex_lock redirect to scePthreadMutexLock");
    int result = scePthreadMutexLock(mutex);
    if (result < 0) {
        int rt = result > SCE_KERNEL_ERROR_UNKNOWN && result <= SCE_KERNEL_ERROR_ESTOP
                     ? result + -SCE_KERNEL_ERROR_UNKNOWN
                     : POSIX_EOTHER;
        return rt;
    }
    return result;
}

int PS4_SYSV_ABI posix_pthread_mutex_unlock(ScePthreadMutex* mutex) {
    // LOG_INFO(Kernel_Pthread, "posix pthread_mutex_unlock redirect to scePthreadMutexUnlock");
    int result = scePthreadMutexUnlock(mutex);
    if (result < 0) {
        int rt = result > SCE_KERNEL_ERROR_UNKNOWN && result <= SCE_KERNEL_ERROR_ESTOP
                     ? result + -SCE_KERNEL_ERROR_UNKNOWN
                     : POSIX_EOTHER;
        return rt;
    }
    return result;
}

int PS4_SYSV_ABI posix_pthread_cond_broadcast(ScePthreadCond* cond) {
    LOG_INFO(Kernel_Pthread,
             "posix posix_pthread_cond_broadcast redirect to scePthreadCondBroadcast");
    int result = scePthreadCondBroadcast(cond);
    if (result != 0) {
        int rt = result > SCE_KERNEL_ERROR_UNKNOWN && result <= SCE_KERNEL_ERROR_ESTOP
                     ? result + -SCE_KERNEL_ERROR_UNKNOWN
                     : POSIX_EOTHER;
        return rt;
    }
    return result;
}

int PS4_SYSV_ABI sceKernelClockGettime(s32 clock_id, SceKernelTimespec* tp) {
    if (tp == nullptr) {
        return SCE_KERNEL_ERROR_EFAULT;
    }
    clockid_t pclock_id = CLOCK_REALTIME;
    switch (clock_id) {
    case 0:
        pclock_id = CLOCK_REALTIME;
        break;
    case 13:
    case 4:
        pclock_id = CLOCK_MONOTONIC;
        break;
    default:
        UNREACHABLE();
    }

    timespec t{};
    int result = clock_gettime(pclock_id, &t);
    tp->tv_sec = t.tv_sec;
    tp->tv_nsec = t.tv_nsec;
    if (result == 0) {
        return SCE_OK;
    }
    return SCE_KERNEL_ERROR_EINVAL;
}

int PS4_SYSV_ABI clock_gettime(s32 clock_id, SceKernelTimespec* time) {
    int result = sceKernelClockGettime(clock_id, time);
    if (result < 0) {
        UNREACHABLE(); // TODO return posix error code
    }
    return result;
}

int PS4_SYSV_ABI sceKernelNanosleep(const SceKernelTimespec* rqtp, SceKernelTimespec* rmtp) {

    if (rqtp == nullptr) {
        return SCE_KERNEL_ERROR_EFAULT;
    }

    if (rqtp->tv_sec < 0 || rqtp->tv_nsec < 0) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    u64 nanos = rqtp->tv_sec * 1000000000 + rqtp->tv_nsec;
    std::this_thread::sleep_for(std::chrono::nanoseconds(nanos));
    if (rmtp != nullptr) {
        UNREACHABLE(); // not supported yet
    }
    return SCE_OK;
}
int PS4_SYSV_ABI nanosleep(const SceKernelTimespec* rqtp, SceKernelTimespec* rmtp) {
    int result = sceKernelNanosleep(rqtp, rmtp);
    if (result < 0) {
        UNREACHABLE(); // TODO return posix error code
    }
    return result;
}
static int pthread_copy_attributes(ScePthreadAttr* dst, const ScePthreadAttr* src) {
    if (dst == nullptr || *dst == nullptr || src == nullptr || *src == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    u64 mask = 0;
    int state = 0;
    size_t guard_size = 0;
    int inherit_sched = 0;
    SceKernelSchedParam param = {};
    int policy = 0;
    void* stack_addr = nullptr;
    size_t stack_size = 0;

    int result = 0;

    result = (result == 0 ? scePthreadAttrGetaffinity(src, &mask) : result);
    result = (result == 0 ? scePthreadAttrGetdetachstate(src, &state) : result);
    result = (result == 0 ? scePthreadAttrGetguardsize(src, &guard_size) : result);
    result = (result == 0 ? scePthreadAttrGetinheritsched(src, &inherit_sched) : result);
    result = (result == 0 ? scePthreadAttrGetschedparam(src, &param) : result);
    result = (result == 0 ? scePthreadAttrGetschedpolicy(src, &policy) : result);
    result = (result == 0 ? scePthreadAttrGetstackaddr(src, &stack_addr) : result);
    result = (result == 0 ? scePthreadAttrGetstacksize(src, &stack_size) : result);

    result = (result == 0 ? scePthreadAttrSetaffinity(dst, mask) : result);
    result = (result == 0 ? scePthreadAttrSetdetachstate(dst, state) : result);
    result = (result == 0 ? scePthreadAttrSetguardsize(dst, guard_size) : result);
    result = (result == 0 ? scePthreadAttrSetinheritsched(dst, inherit_sched) : result);
    result = (result == 0 ? scePthreadAttrSetschedparam(dst, &param) : result);
    result = (result == 0 ? scePthreadAttrSetschedpolicy(dst, policy) : result);
    if (stack_addr != nullptr) {
        result = (result == 0 ? scePthreadAttrSetstackaddr(dst, stack_addr) : result);
    }
    if (stack_size != 0) {
        result = (result == 0 ? scePthreadAttrSetstacksize(dst, stack_size) : result);
    }

    return result;
}

int PS4_SYSV_ABI scePthreadAttrGet(ScePthread thread, ScePthreadAttr* attr) {
    if (thread == nullptr || attr == nullptr || *attr == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    return pthread_copy_attributes(attr, &thread->attr);
}

static void cleanup_thread(void* arg) {
    auto* thread = static_cast<ScePthread>(arg);
    thread->is_almost_done = true;
}

static void* run_thread(void* arg) {
    auto* thread = static_cast<ScePthread>(arg);
    void* ret = nullptr;
    g_pthread_self = thread;
    pthread_cleanup_push(cleanup_thread, thread);
    thread->is_started = true;
    ret = thread->entry(thread->arg);
    pthread_cleanup_pop(1);
    return ret;
}

int PS4_SYSV_ABI scePthreadCreate(ScePthread* thread, const ScePthreadAttr* attr,
                                  pthreadEntryFunc start_routine, void* arg, const char* name) {
    if (thread == nullptr) {
        return SCE_KERNEL_ERROR_EINVAL;
    }

    auto* pthread_pool = g_pthread_cxt->GetPthreadPool();

    if (attr == nullptr) {
        attr = g_pthread_cxt->GetDefaultAttr();
    }

    *thread = pthread_pool->Create();

    if ((*thread)->attr != nullptr) {
        scePthreadAttrDestroy(&(*thread)->attr);
    }

    scePthreadAttrInit(&(*thread)->attr);

    int result = pthread_copy_attributes(&(*thread)->attr, attr);

    if (result == 0) {
        if (name != NULL) {
            (*thread)->name = name;
        } else {
            (*thread)->name = "no-name";
        }
        (*thread)->entry = start_routine;
        (*thread)->arg = arg;
        (*thread)->is_almost_done = false;
        (*thread)->is_detached = (*attr)->detached;
        (*thread)->is_started = false;

        result = pthread_create(&(*thread)->pth, &(*attr)->pth_attr, run_thread, *thread);
    }

    if (result == 0) {
        while (!(*thread)->is_started) {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }
    }
    LOG_INFO(Kernel_Pthread, "thread create name = {}", (*thread)->name);

    switch (result) {
    case 0:
        return SCE_OK;
    case ENOMEM:
        return SCE_KERNEL_ERROR_ENOMEM;
    case EAGAIN:
        return SCE_KERNEL_ERROR_EAGAIN;
    case EDEADLK:
        return SCE_KERNEL_ERROR_EDEADLK;
    case EPERM:
        return SCE_KERNEL_ERROR_EPERM;
    default:
        return SCE_KERNEL_ERROR_EINVAL;
    }
}

ScePthread PThreadPool::Create() {
    std::scoped_lock lock{m_mutex};

    for (auto* p : m_threads) {
        if (p->is_free) {
            p->is_free = false;
            return p;
        }
    }

    auto* ret = new PthreadInternal{};

    ret->is_free = false;
    ret->is_detached = false;
    ret->is_almost_done = false;
    ret->attr = nullptr;

    m_threads.push_back(ret);

    return ret;
}

void PS4_SYSV_ABI scePthreadYield() {
    sched_yield();
}

void pthreadSymbolsRegister(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("4+h9EzwKF4I", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrSetschedpolicy);
    LIB_FUNCTION("-Wreprtu0Qs", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrSetdetachstate);
    LIB_FUNCTION("eXbUSpEaTsA", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrSetinheritsched);
    LIB_FUNCTION("DzES9hQF4f4", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrSetschedparam);
    LIB_FUNCTION("nsYoNRywwNg", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrInit);
    LIB_FUNCTION("62KCwEMmzcM", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrDestroy);

    LIB_FUNCTION("aI+OeCz8xrQ", "libkernel", 1, "libkernel", 1, 1, scePthreadSelf);
    LIB_FUNCTION("3qxgM4ezETA", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrSetaffinity);
    LIB_FUNCTION("8+s5BzZjxSg", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrGetaffinity);
    LIB_FUNCTION("x1X76arYMxU", "libkernel", 1, "libkernel", 1, 1, scePthreadAttrGet);

    LIB_FUNCTION("bt3CTBKmGyI", "libkernel", 1, "libkernel", 1, 1, scePthreadSetaffinity);
    LIB_FUNCTION("6UgtwV+0zb4", "libkernel", 1, "libkernel", 1, 1, scePthreadCreate);
    LIB_FUNCTION("T72hz6ffq08", "libkernel", 1, "libkernel", 1, 1, scePthreadYield);

    // mutex calls
    LIB_FUNCTION("cmo1RIYva9o", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexInit);
    LIB_FUNCTION("2Of0f+3mhhE", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexDestroy);
    LIB_FUNCTION("F8bUHwAG284", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexattrInit);
    LIB_FUNCTION("smWEktiyyG0", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexattrDestroy);
    LIB_FUNCTION("iMp8QpE+XO4", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexattrSettype);
    LIB_FUNCTION("1FGvU0i9saQ", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexattrSetprotocol);
    LIB_FUNCTION("9UK1vLZQft4", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexLock);
    LIB_FUNCTION("tn3VlD0hG60", "libkernel", 1, "libkernel", 1, 1, scePthreadMutexUnlock);
    // cond calls
    LIB_FUNCTION("2Tb92quprl0", "libkernel", 1, "libkernel", 1, 1, scePthreadCondInit);
    LIB_FUNCTION("m5-2bsNfv7s", "libkernel", 1, "libkernel", 1, 1, scePthreadCondattrInit);
    LIB_FUNCTION("JGgj7Uvrl+A", "libkernel", 1, "libkernel", 1, 1, scePthreadCondBroadcast);
    // posix calls
    LIB_FUNCTION("ttHNfU+qDBU", "libScePosix", 1, "libkernel", 1, 1, posix_pthread_mutex_init);
    LIB_FUNCTION("7H0iTOciTLo", "libScePosix", 1, "libkernel", 1, 1, posix_pthread_mutex_lock);
    LIB_FUNCTION("2Z+PpY6CaJg", "libScePosix", 1, "libkernel", 1, 1, posix_pthread_mutex_unlock);
    LIB_FUNCTION("mkx2fVhNMsg", "libScePosix", 1, "libkernel", 1, 1, posix_pthread_cond_broadcast);

    LIB_FUNCTION("QBi7HCK03hw", "libkernel", 1, "libkernel", 1, 1, sceKernelClockGettime);
    LIB_FUNCTION("lLMT9vJAck0", "libkernel", 1, "libkernel", 1, 1, clock_gettime);
    LIB_FUNCTION("yS8U2TGCe1A", "libScePosix", 1, "libkernel", 1, 1, nanosleep);

    // openorbis weird functions
    LIB_FUNCTION("7H0iTOciTLo", "libkernel", 1, "libkernel", 1, 1, posix_pthread_mutex_lock);
    LIB_FUNCTION("2Z+PpY6CaJg", "libkernel", 1, "libkernel", 1, 1, posix_pthread_mutex_unlock);
    LIB_FUNCTION("mkx2fVhNMsg", "libkernel", 1, "libkernel", 1, 1, posix_pthread_cond_broadcast);
}

} // namespace Libraries::Kernel
