/*
 * Copyright (C) 2016 The CyanogenMod Project
 *               2017-2021 The LineageOS Project
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

#include "livedisplay/sdm/Utils.h"
#include <android/hidl/manager/1.0/IServiceManager.h>
#include <hidl/ServiceManagement.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {
namespace utils {

status_t CheckFeatureVersion(const std::shared_ptr<SDMController>& controller,
                             FeatureVerSw feature) {
    SdmFeatureVersion version{};
    status_t status = controller->getFeatureVersion(feature, &version);
    if (status != android::OK) {
        return status;
    }

    if (version.x <= 0 && version.y <= 0 && version.z <= 0) {
        return android::BAD_VALUE;
    }

    return android::OK;
}

bool IsEnabledInManifest(const hidl_string& fq_name, const hidl_string& name) {
    using ::android::hidl::manager::V1_0::IServiceManager;
    auto sm = ::android::hardware::defaultServiceManager();
    auto transport = sm->getTransport(fq_name, name);
    return transport != IServiceManager::Transport::EMPTY;
}

}  // namespace utils
}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
