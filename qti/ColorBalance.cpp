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

#include "ColorBalance.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace qti {

ColorBalance::ColorBalance() {
    mColorHal = IColor::getService();
}

bool ColorBalance::isSupported() {
    if (mColorHal != nullptr) {
        return mColorHal->getSupportedFeatures() & Feature::COLOR_BALANCE;
    }

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IColorBalance follow.
Return<void> ColorBalance::getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) {
    if (mColorHal != nullptr) {
        return mColorHal->getColorBalanceRange(_hidl_cb);
    }

    return Void();
}

Return<int32_t> ColorBalance::getColorBalance() {
    if (mColorHal != nullptr) {
        return mColorHal->getColorBalance();
    }

    return 0;
}

Return<bool> ColorBalance::setColorBalance(int32_t value) {
    if (mColorHal != nullptr) {
        return mColorHal->setColorBalance(value);
    }

    return false;
}

}  // namespace qti
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
