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

#include "AdaptiveBacklight.h"

#include <android-base/properties.h>
#include <stdio.h>
#include <string.h>

#include "Constants.h"
#include "Types.h"
#include "Utils.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

using ::android::base::GetBoolProperty;

AdaptiveBacklight::AdaptiveBacklight() {
    mEnabled = false;
}

bool AdaptiveBacklight::isSupported() {
    return GetBoolProperty(FOSS_PROPERTY, false);
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> AdaptiveBacklight::isEnabled() {
    return mEnabled;
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    if (mEnabled == enabled) {
        return true;
    }

    auto buf = std::make_unique<char[]>(DPPS_BUF_SIZE);

    sprintf(buf.get(), "%s", enabled ? FOSS_ON : FOSS_OFF);
    if (Utils::sendDPPSCommand(buf.get(), DPPS_BUF_SIZE) == 0) {
        if (strncmp(buf.get(), "Success", 7) == 0) {
            mEnabled = enabled;
            return true;
        }
    }

    return false;
}

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
