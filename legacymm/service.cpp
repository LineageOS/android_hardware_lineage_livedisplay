/*
 * Copyright (C) 2018-2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>

#define LOG_TAG "vendor.lineage.livedisplay@2.0-service-legacymm"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "ColorBalance.h"
#include "DisplayModes.h"
#include "PictureAdjustment.h"

#define MM_DISP_LIB "libmm-disp-apis.so"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;
using android::status_t;
using android::OK;

using ::vendor::lineage::livedisplay::V2_0::IColorBalance;
using ::vendor::lineage::livedisplay::V2_0::legacymm::ColorBalance;
using ::vendor::lineage::livedisplay::V2_0::IDisplayModes;
using ::vendor::lineage::livedisplay::V2_0::legacymm::DisplayModes;
using ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::legacymm::PictureAdjustment;

int main() {
    // Vendor backend
    void* libHandle = nullptr;
    int (*disp_api_init)(int32_t) = nullptr;

    // HIDL frontend
    sp<ColorBalance> cb;
    sp<DisplayModes> dm;
    sp<PictureAdjustment> pa;
    uint8_t services = 0;

    status_t status = OK;

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    libHandle = dlopen(MM_DISP_LIB, RTLD_NOW);
    if (libHandle == nullptr) {
        LOG(ERROR) << "Can not get " << MM_DISP_LIB << " (" << dlerror() << ")";
        goto shutdown;
    }

    disp_api_init = reinterpret_cast<int (*)(int32_t)>(dlsym(libHandle, "disp_api_init"));
    if (disp_api_init == nullptr) {
        LOG(ERROR) << "Can not get disp_api_init from " << MM_DISP_LIB << " (" << dlerror() << ")";
        goto shutdown;
    }

    status = disp_api_init(0);
    if (status != OK) {
        LOG(ERROR) << "Can not initialize " << MM_DISP_LIB << " (" << status << ")";
        goto shutdown;
    }

    cb = new ColorBalance(libHandle);
    if (cb == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL ColorBalance "
                      "Iface, exiting.";
        goto shutdown;
    }
    if (cb->isSupported()) {
        services++;
    }

    dm = new DisplayModes(libHandle);
    if (dm == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL DisplayModes "
                      "Iface, exiting.";
        goto shutdown;
    }
    if (dm->isSupported()) {
        services++;
    }

    pa = new PictureAdjustment(libHandle);
    if (pa == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL "
                      "PictureAdjustment Iface, exiting.";
        goto shutdown;
    }
    if (pa->isSupported()) {
        services++;
    }

    configureRpcThreadpool(services, true /*callerWillJoin*/);

    if (cb->isSupported()) {
        status = cb->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL "
                          "ColorBalance Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    if (dm->isSupported()) {
        status = dm->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL "
                          "DisplayModes Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    if (pa->isSupported()) {
        status = pa->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL "
                          "PictureAdjustment Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // Cleanup what we started
    if (disp_api_init != nullptr) {
        disp_api_init(1);
    }

    if (libHandle != nullptr) {
        dlclose(libHandle);
    }

    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return 1;
}
