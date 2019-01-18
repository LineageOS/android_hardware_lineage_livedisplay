/*
 * Copyright (C) 2018 The LineageOS Project
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

#include "SunlightEnhancement.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace qti {

SunlightEnhancement::SunlightEnhancement() {
    mColorHal = IColor::getService();
}

bool SunlightEnhancement::isSupported() {
    if (mColorHal != nullptr) {
        return mColorHal->getSupportedFeatures() & Feature::OUTDOOR_MODE;
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::ISunlightEnhancement follow.
Return<bool> SunlightEnhancement::isEnabled() {
    if (mColorHal != nullptr) {
        return mColorHal->isOutdoorModeEnabled();
    }

    return false;
}

Return<bool> SunlightEnhancement::setEnabled(bool enabled) {
    if (mColorHal != nullptr) {
        return mColorHal->setOutdoorModeEnabled(enabled);
    }

    return false;
}

}  // namespace qti
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
