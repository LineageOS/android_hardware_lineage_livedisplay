/*
 * Copyright (C) 2019-2020 The LineageOS Project
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

#ifdef LIVES_IN_SYSTEM
#define LOG_TAG "lineage.livedisplay@2.0-impl-sdm"
#else
#define LOG_TAG "vendor.lineage.livedisplay@2.0-impl-sdm"
#endif

#include "DisplayModes.h"

#include <android-base/logging.h>
#include <android/hidl/manager/1.0/IServiceManager.h>
#include <hidl/ServiceManagement.h>

#include "Constants.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

using ::android::OK;

DisplayModes::DisplayModes(std::shared_ptr<SDMController> controller)
    : controller_(std::move(controller)) {
    if (!isSupported()) {
        // Terminate the program if unsupported
        LOG(FATAL) << "Failed to initialize DisplayModes";
    }

#ifdef LIVES_IN_SYSTEM
    DisplayMode mode = getDefaultDisplayModeInternal();
    if (mode.id >= 0) {
        setDisplayMode(mode.id, false);
    }
#endif
}

bool DisplayModes::isEnabledFromManifest(const hidl_string& name) {
    using ::android::hidl::manager::V1_0::IServiceManager;
    auto sm = ::android::hardware::defaultServiceManager();
    /*
     * We MUST NOT use DisplayModes::isSupported to check the availability here,
     * but check the existence in manifest instead. Because in certain cases the
     * QDCM backend might fail to initialize thus isSupported would return false,
     * even though the device does support the feature and the interface is
     * declared in manifest. Under the circumstance, the HAL will abort and
     * hopefully recover from the failure when started again.
     */
    auto transport = sm->getTransport(descriptor, name);
    return transport != IServiceManager::Transport::EMPTY;
}

bool DisplayModes::isSupported() {
    static int supported = -1;

    if (supported >= 0) {
        return supported;
    }

    SdmFeatureVersion version{};
    if (controller_->getFeatureVersion(DISPLAY_MODES_FEATURE, &version) != OK) {
        supported = 0;
        return false;
    }

    if (version.x <= 0 && version.y <= 0 && version.z <= 0) {
        supported = 0;
        return false;
    }

    int32_t count = 0;
    if (controller_->getNumDisplayModes(&count) != OK) {
        count = 0;
    }
    supported = (count > 0);

    return supported;
}

std::vector<DisplayMode> DisplayModes::getDisplayModesInternal() {
    std::vector<DisplayMode> modes;
    int32_t count = 0;

    if (controller_->getNumDisplayModes(&count) != OK || count == 0) {
        return modes;
    }

    std::vector<SdmDispMode> tmp_modes(count);
    if (controller_->getDisplayModes(tmp_modes.data(), count) == OK) {
        for (auto&& mode : tmp_modes) {
            modes.push_back({mode.id, mode.name});
        }
    }

    return modes;
}

DisplayMode DisplayModes::getDisplayModeById(int32_t id) {
    std::vector<DisplayMode> modes = getDisplayModesInternal();

    for (const DisplayMode& mode : modes) {
        if (mode.id == id) {
            return mode;
        }
    }

    return DisplayMode{-1, ""};
}

DisplayMode DisplayModes::getCurrentDisplayModeInternal() {
    int32_t id = 0;

    if (controller_->getActiveDisplayMode(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return DisplayMode{-1, ""};
}

DisplayMode DisplayModes::getDefaultDisplayModeInternal() {
    int32_t id = 0;

    if (controller_->getDefaultDisplayMode(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return DisplayMode{-1, ""};
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
Return<void> DisplayModes::getDisplayModes(getDisplayModes_cb _hidl_cb) {
    _hidl_cb(getDisplayModesInternal());
    return Void();
}

Return<void> DisplayModes::getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) {
    _hidl_cb(getCurrentDisplayModeInternal());
    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) {
    _hidl_cb(getDefaultDisplayModeInternal());
    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t mode_id, bool make_default) {
    DisplayMode current_mode = getCurrentDisplayModeInternal();

    if (current_mode.id >= 0 && current_mode.id == mode_id) {
        return true;
    }

    DisplayMode mode = getDisplayModeById(mode_id);
    if (mode.id < 0) {
        return false;
    }

    if (controller_->setActiveDisplayMode(mode_id) != OK) {
        return false;
    }

    if (make_default && controller_->setDefaultDisplayMode(mode_id) != OK) {
        return false;
    }

    if (on_display_mode_set_) {
        on_display_mode_set_();
    }

    return true;
}

void DisplayModes::registerDisplayModeSetCallback(DisplayModeSetCallback callback) {
    on_display_mode_set_ = callback;
}

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
