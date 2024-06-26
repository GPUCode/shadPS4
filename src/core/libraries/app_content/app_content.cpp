// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

// Generated By moduleGenerator
#include <cmath>
#include <common/path_util.h>
#include <common/singleton.h>
#include <core/file_format/psf.h>
#include <core/file_sys/fs.h>
#include "app_content.h"
#include "common/io_file.h"
#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/libs.h"

namespace Libraries::AppContent {

int PS4_SYSV_ABI _Z5dummyv() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontDelete() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontEnqueueDownload() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontEnqueueDownloadSp() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontMount() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontShrink() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontUnmount() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAppParamGetInt(OrbisAppContentAppParamId paramId, s32* value) {
    if (value == nullptr)
        return 0x80D90002;
    auto* param_sfo = Common::Singleton<PSF>::Instance();
    switch (paramId) {
    case ORBIS_APP_CONTENT_APPPARAM_ID_SKU_FLAG:
        *value = ORBIS_APP_CONTENT_APPPARAM_SKU_FLAG_FULL;
        break;
    case ORBIS_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_1:
        *value = param_sfo->GetInteger("USER_DEFINED_PARAM_1");
        break;
    case ORBIS_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_2:
        *value = param_sfo->GetInteger("USER_DEFINED_PARAM_2");
        break;
    case ORBIS_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_3:
        *value = param_sfo->GetInteger("USER_DEFINED_PARAM_3");
        break;
    case ORBIS_APP_CONTENT_APPPARAM_ID_USER_DEFINED_PARAM_4:
        *value = param_sfo->GetInteger("USER_DEFINED_PARAM_4");
        break;
    default:
        LOG_ERROR(Lib_AppContent, " paramId = {}, value = {}", paramId, *value);
        return *value == -1 ? 0x80D90005 : ORBIS_OK;
    }
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAppParamGetString() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentDownload0Expand() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentDownload0Shrink() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentDownload1Expand() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentDownload1Shrink() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentDownloadDataFormat() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentDownloadDataGetAvailableSpaceKb() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetAddcontDownloadProgress() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetAddcontInfo() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetAddcontInfoList() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetEntitlementKey() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetRegion() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentInitialize(const OrbisAppContentInitParam* initParam,
                                         OrbisAppContentBootParam* bootParam) {
    LOG_ERROR(Lib_AppContent, "(DUMMY) called");
    bootParam->attr = 0; // always 0
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentRequestPatchInstall() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentSmallSharedDataFormat() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentSmallSharedDataGetAvailableSpaceKb() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentSmallSharedDataMount() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentSmallSharedDataUnmount() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentTemporaryDataFormat() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentTemporaryDataGetAvailableSpaceKb(
    const OrbisAppContentMountPoint* mountPoint, size_t* availableSpaceKb) {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    *availableSpaceKb = 1073741824;
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentTemporaryDataMount() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentTemporaryDataMount2(OrbisAppContentTemporaryDataOption option,
                                                  OrbisAppContentMountPoint* mountPoint) {
    auto* param_sfo = Common::Singleton<PSF>::Instance();
    std::string id(param_sfo->GetString("CONTENT_ID"), 7, 9);
    const auto& mount_dir = Common::FS::GetUserPath(Common::FS::PathType::TempDataDir) / id;
    auto* mnt = Common::Singleton<Core::FileSys::MntPoints>::Instance();
    mnt->Mount(mount_dir, mountPoint->data);
    LOG_INFO(Lib_AppContent, "sceAppContentTemporaryDataMount2: option = {}, mountPoint = {}",
             option, mountPoint->data);
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentTemporaryDataUnmount() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetPftFlag() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI Func_C59A36FF8D7C59DA() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontEnqueueDownloadByEntitlemetId() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentAddcontMountByEntitlemetId() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetAddcontInfoByEntitlementId() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetAddcontInfoListByIroTag() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAppContentGetDownloadedStoreCountry() {
    LOG_ERROR(Lib_AppContent, "(STUBBED) called");
    return ORBIS_OK;
}

void RegisterlibSceAppContent(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("AS45QoYHjc4", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1, _Z5dummyv);
    LIB_FUNCTION("ZiATpP9gEkA", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAddcontDelete);
    LIB_FUNCTION("7gxh+5QubhY", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAddcontEnqueueDownload);
    LIB_FUNCTION("TVM-aYIsG9k", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAddcontEnqueueDownloadSp);
    LIB_FUNCTION("VANhIWcqYak", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAddcontMount);
    LIB_FUNCTION("D3H+cjfzzFY", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAddcontShrink);
    LIB_FUNCTION("3rHWaV-1KC4", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAddcontUnmount);
    LIB_FUNCTION("99b82IKXpH4", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAppParamGetInt);
    LIB_FUNCTION("+OlXCu8qxUk", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentAppParamGetString);
    LIB_FUNCTION("gpGZDB4ZlrI", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentDownload0Expand);
    LIB_FUNCTION("S5eMvWnbbXg", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentDownload0Shrink);
    LIB_FUNCTION("B5gVeVurdUA", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentDownload1Expand);
    LIB_FUNCTION("kUeYucqnb7o", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentDownload1Shrink);
    LIB_FUNCTION("CN7EbEV7MFU", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentDownloadDataFormat);
    LIB_FUNCTION("Gl6w5i0JokY", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentDownloadDataGetAvailableSpaceKb);
    LIB_FUNCTION("5bvvbUSiFs4", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentGetAddcontDownloadProgress);
    LIB_FUNCTION("m47juOmH0VE", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentGetAddcontInfo);
    LIB_FUNCTION("xnd8BJzAxmk", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentGetAddcontInfoList);
    LIB_FUNCTION("XTWR0UXvcgs", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentGetEntitlementKey);
    LIB_FUNCTION("74-1x3lyZK8", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentGetRegion);
    LIB_FUNCTION("R9lA82OraNs", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentInitialize);
    LIB_FUNCTION("bVtF7v2uqT0", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentRequestPatchInstall);
    LIB_FUNCTION("9Gq5rOkWzNU", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentSmallSharedDataFormat);
    LIB_FUNCTION("xhb-r8etmAA", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentSmallSharedDataGetAvailableSpaceKb);
    LIB_FUNCTION("QuApZnMo9MM", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentSmallSharedDataMount);
    LIB_FUNCTION("EqMtBHWu-5M", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentSmallSharedDataUnmount);
    LIB_FUNCTION("a5N7lAG0y2Q", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentTemporaryDataFormat);
    LIB_FUNCTION("SaKib2Ug0yI", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentTemporaryDataGetAvailableSpaceKb);
    LIB_FUNCTION("7bOLX66Iz-U", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentTemporaryDataMount);
    LIB_FUNCTION("buYbeLOGWmA", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentTemporaryDataMount2);
    LIB_FUNCTION("bcolXMmp6qQ", "libSceAppContent", 1, "libSceAppContentUtil", 1, 1,
                 sceAppContentTemporaryDataUnmount);
    LIB_FUNCTION("xmhnAoxN3Wk", "libSceAppContentPft", 1, "libSceAppContent", 1, 1,
                 sceAppContentGetPftFlag);
    LIB_FUNCTION("xZo2-418Wdo", "libSceAppContentBundle", 1, "libSceAppContent", 1, 1,
                 Func_C59A36FF8D7C59DA);
    LIB_FUNCTION("kJmjt81mXKQ", "libSceAppContentIro", 1, "libSceAppContent", 1, 1,
                 sceAppContentAddcontEnqueueDownloadByEntitlemetId);
    LIB_FUNCTION("efX3lrPwdKA", "libSceAppContentIro", 1, "libSceAppContent", 1, 1,
                 sceAppContentAddcontMountByEntitlemetId);
    LIB_FUNCTION("z9hgjLd1SGA", "libSceAppContentIro", 1, "libSceAppContent", 1, 1,
                 sceAppContentGetAddcontInfoByEntitlementId);
    LIB_FUNCTION("3wUaDTGmjcQ", "libSceAppContentIro", 1, "libSceAppContent", 1, 1,
                 sceAppContentGetAddcontInfoListByIroTag);
    LIB_FUNCTION("TCqT7kPuGx0", "libSceAppContentSc", 1, "libSceAppContent", 1, 1,
                 sceAppContentGetDownloadedStoreCountry);
};

} // namespace Libraries::AppContent
