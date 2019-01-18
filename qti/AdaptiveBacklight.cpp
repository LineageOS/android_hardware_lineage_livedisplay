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

#include "AdaptiveBacklight.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace qti {

AdaptiveBacklight::AdaptiveBacklight() {
    mColorHal = IColor::getService();
}

bool AdaptiveBacklight::isSupported() {
    if (mColorHal != nullptr) {
        return mColorHal->getSupportedFeatures() & Feature::ADAPTIVE_BACKLIGHT;
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> AdaptiveBacklight::isEnabled() {
    if (mColorHal != nullptr) {
        return mColorHal->isAdaptiveBacklightEnabled();
    }

    return false;
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    if (mColorHal != nullptr) {
        return mColorHal->setAdaptiveBacklightEnabled(enabled);
    }

    return false;
}

}  // namespace qti
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
