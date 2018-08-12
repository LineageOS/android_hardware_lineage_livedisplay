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

#include "PictureAdjustment.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace qti {

PictureAdjustment::PictureAdjustment() {
    mColorHal = IColor::getService();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment follow.
Return<void> PictureAdjustment::getHueRange(getHueRange_cb _hidl_cb) {
    if (mColorHal != nullptr) {
       return mColorHal->getHueRange(_hidl_cb);
    }

    return Void();
}

Return<void> PictureAdjustment::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    if (mColorHal != nullptr) {
       return mColorHal->getSaturationRange(_hidl_cb);
    }

    return Void();
}

Return<void> PictureAdjustment::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    if (mColorHal != nullptr) {
       return mColorHal->getIntensityRange(_hidl_cb);
    }

    return Void();
}

Return<void> PictureAdjustment::getContrastRange(getContrastRange_cb _hidl_cb) {
    if (mColorHal != nullptr) {
       return mColorHal->getContrastRange(_hidl_cb);
    }

    return Void();
}

Return<void> PictureAdjustment::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
    if (mColorHal != nullptr) {
        return mColorHal->getSaturationThresholdRange(_hidl_cb);
    }

    return Void();
}

Return<void> PictureAdjustment::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
    if (mColorHal != nullptr) {
        return mColorHal->getPictureAdjustment(_hidl_cb);
    }

    return Void();
}

Return<void> PictureAdjustment::getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) {
    if (mColorHal != nullptr) {
        return mColorHal->getDefaultPictureAdjustment(_hidl_cb);
    }

    return Void();
}

Return<bool> PictureAdjustment::setPictureAdjustment(const ::vendor::lineage::livedisplay::V1_0::HSIC& hsic) {
    if (mColorHal != nullptr) {
        return mColorHal->setPictureAdjustment(hsic);
    }

    return false;
}

}  // namespace qti
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
