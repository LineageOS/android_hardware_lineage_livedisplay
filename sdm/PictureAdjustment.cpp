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

#include "PictureAdjustment.h"

#include <android-base/logging.h>

#include "Constants.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

using ::android::OK;
static sp<PictureAdjustment> sInstance;

PictureAdjustment::PictureAdjustment(std::shared_ptr<SDMController> controller)
    : controller_(std::move(controller)) {
    if (!isSupported()) {
        // Terminate the program if unsupported
        LOG(FATAL) << "Failed to initialize DisplayModes";
    }
    sInstance = this;

    memset(&mDefaultPictureAdjustment, 0, sizeof(HSIC));
}

bool PictureAdjustment::isSupported() {
    static int supported = -1;

    if (supported >= 0) {
        return supported;
    }

    sdm_feature_version version{};
    if (controller_->getFeatureVersion(PICTURE_ADJUSTMENT_FEATURE, &version) != OK) {
        supported = 0;
        return false;
    }

    if (version.x <= 0 && version.y <= 0 && version.z <= 0) {
        supported = 0;
        return false;
    }

    hsic_ranges r{};
    if (controller_->getGlobalPaRange(&r) == OK) {
        supported = r.hue.max != 0 && r.hue.min != 0 && r.saturation.max != 0.f &&
                    r.saturation.min != 0.f && r.intensity.max != 0.f && r.intensity.min != 0.f &&
                    r.contrast.max != 0.f && r.contrast.min != 0.f;
    } else {
        supported = 0;
    }

    return supported;
}

HSIC PictureAdjustment::getPictureAdjustmentInternal() {
    hsic_config config{};

    if (controller_->getGlobalPaConfig(&config) == OK) {
        return HSIC{static_cast<float>(config.data.hue), config.data.saturation,
                    config.data.intensity, config.data.contrast, config.data.saturationThreshold};
    }

    return HSIC{};
}

void PictureAdjustment::updateDefaultPictureAdjustment() {
    if (sInstance != nullptr) {
        sInstance->mDefaultPictureAdjustment = sInstance->getPictureAdjustmentInternal();
    }
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment follow.
Return<void> PictureAdjustment::getHueRange(getHueRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.hue.max;
        range.min = r.hue.min;
        range.step = r.hue.step;
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.saturation.max;
        range.min = r.saturation.min;
        range.step = r.saturation.step;
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.intensity.max;
        range.min = r.intensity.min;
        range.step = r.intensity.step;
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getContrastRange(getContrastRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.contrast.max;
        range.min = r.contrast.min;
        range.step = r.contrast.step;
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getSaturationThresholdRange(
        getSaturationThresholdRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (controller_->getGlobalPaRange(&r) == OK) {
        range.max = r.saturationThreshold.max;
        range.min = r.saturationThreshold.min;
        range.step = r.saturationThreshold.step;
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
    _hidl_cb(getPictureAdjustmentInternal());
    return Void();
}

Return<void> PictureAdjustment::getDefaultPictureAdjustment(
        getDefaultPictureAdjustment_cb _hidl_cb) {
    _hidl_cb(mDefaultPictureAdjustment);
    return Void();
}

Return<bool> PictureAdjustment::setPictureAdjustment(
        const ::vendor::lineage::livedisplay::V2_0::HSIC& hsic) {
    hsic_config config = {0,
                          {static_cast<int32_t>(hsic.hue), hsic.saturation, hsic.intensity,
                           hsic.contrast, hsic.saturationThreshold}};

    return controller_->setGlobalPaConfig(&config) == OK;
}

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
