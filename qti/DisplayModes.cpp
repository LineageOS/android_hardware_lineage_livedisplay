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

#include "DisplayModes.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace qti {

DisplayModes::DisplayModes() {
    mColorHal = IColor::getService();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
Return<void> DisplayModes::getDisplayModes(getDisplayModes_cb _hidl_cb) {
    if (mColorHal != nullptr) {
        return mColorHal->getDisplayModes(_hidl_cb);
    }

    return Void();
}

Return<void> DisplayModes::getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) {
    if (mColorHal != nullptr) {
        return mColorHal->getCurrentDisplayMode(_hidl_cb);
    }

    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) {
    if (mColorHal != nullptr) {
        return mColorHal->getDefaultDisplayMode(_hidl_cb);
    }

    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    if (mColorHal != nullptr) {
        return mColorHal->setDisplayMode(modeID, makeDefault);
    }

    return false;
}

}  // namespace qti
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
