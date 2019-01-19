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
#include <string.h>

#include "PictureAdjustment.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace legacymm {

static sp<PictureAdjustment> sInstance;

PictureAdjustment::PictureAdjustment(void *libHandle) {
    mLibHandle = libHandle;
    disp_api_supported = (int(*)(int32_t, int32_t))dlsym(mLibHandle, "disp_api_supported");
    disp_api_get_pa_range = (int(*)(int32_t, void*))dlsym(mLibHandle, "disp_api_get_pa_range");
    disp_api_get_pa_config =
        (int(*)(int32_t, void*))dlsym(mLibHandle, "disp_api_get_pa_config");
    disp_api_set_pa_config =
        (int(*)(int32_t, void*))dlsym(mLibHandle, "disp_api_set_pa_config");
    mDefaultPictureAdjustment = HSIC{0, 0.f, 0.f, 0.f, 0.f};
    sInstance = this;
}

bool PictureAdjustment::isSupported() {
    if (disp_api_supported != NULL) {
        if (disp_api_supported(0, PICTURE_ADJUSTMENT_FEATURE)) {
            struct mm_pa_range r;

            memset(&r, 0, sizeof(struct mm_pa_range));

            if (disp_api_get_pa_range != NULL) {
                if (disp_api_get_pa_range(0, &r) == 0) {
                    if (r.max.hue != 0 && r.min.hue != 0 && r.max.saturation != 0.f &&
                            r.min.saturation != 0.f && r.max.intensity != 0.f &&
                            r.min.intensity != 0.f && r.max.contrast != 0.f &&
                            r.min.contrast != 0.f && r.max.saturationThreshold != 0.f &&
                            r.min.saturationThreshold != 0.f) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

HSIC PictureAdjustment::getPictureAdjustmentInternal() {
    struct mm_pa_config config;

    memset(&config, 0, sizeof(struct mm_pa_config));

    if (disp_api_get_pa_config != NULL) {
        if (disp_api_get_pa_config(0, &config) == 0) {
            return HSIC{config.data.hue, (float)config.data.saturation,
                    (float)config.data.intensity, (float)config.data.contrast,
                    (float)config.data.saturationThreshold};
        }
    }

    return HSIC{0, 0.f, 0.f, 0.f, 0.f};
}

void PictureAdjustment::updateDefaultPictureAdjustment() {
    if (sInstance != nullptr) {
        sInstance->mDefaultPictureAdjustment = sInstance->getPictureAdjustmentInternal();
    }
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment follow.
Return<void> PictureAdjustment::getHueRange(getHueRange_cb _hidl_cb) {
    Range range;
    struct mm_pa_range r;

    range.max = range.min = 0;
    memset(&r, 0, sizeof(struct mm_pa_range));

    if (disp_api_get_pa_range != NULL) {
        if (disp_api_get_pa_range(0, &r) == 0) {
            range.max = r.max.hue;
            range.min = r.min.hue;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    FloatRange range;
    struct mm_pa_range r;

    range.max = range.min = 0.f;
    memset(&r, 0, sizeof(struct mm_pa_range));

    if (disp_api_get_pa_range != NULL) {
        if (disp_api_get_pa_range(0, &r) == 0) {
            range.max = r.max.saturation;
            range.min = r.min.saturation;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    FloatRange range;
    struct mm_pa_range r;

    range.max = range.min = 0.f;
    memset(&r, 0, sizeof(struct mm_pa_range));

    if (disp_api_get_pa_range != NULL) {
        if (disp_api_get_pa_range(0, &r) == 0) {
            range.max = r.max.intensity;
            range.min = r.min.intensity;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getContrastRange(getContrastRange_cb _hidl_cb) {
    FloatRange range;
    struct mm_pa_range r;

    range.max = range.min = 0.f;
    memset(&r, 0, sizeof(struct mm_pa_range));

    if (disp_api_get_pa_range != NULL) {
        if (disp_api_get_pa_range(0, &r) == 0) {
            range.max = r.max.contrast;
            range.min = r.min.contrast;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
    FloatRange range;
    struct mm_pa_range r;

    range.max = range.min = 0.f;
    memset(&r, 0, sizeof(struct mm_pa_range));

    if (disp_api_get_pa_range != NULL) {
        if (disp_api_get_pa_range(0, &r) == 0) {
            range.max = r.max.saturationThreshold;
            range.min = r.min.saturationThreshold;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
    _hidl_cb(getPictureAdjustmentInternal());
    return Void();
}

Return<void> PictureAdjustment::getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) {
    _hidl_cb(mDefaultPictureAdjustment);
    return Void();
}

Return<bool> PictureAdjustment::setPictureAdjustment(const ::vendor::lineage::livedisplay::V2_0::HSIC& hsic) {
    struct mm_pa_config config;

    memset(&config, 0, sizeof(struct mm_pa_config));

    config.flags = 0xF;
    config.data.hue = hsic.hue;
    config.data.saturation = hsic.saturation;
    config.data.intensity = hsic.intensity;
    config.data.contrast = hsic.contrast;
    config.data.saturationThreshold = hsic.saturationThreshold;

    if (disp_api_set_pa_config != NULL) {
        return disp_api_set_pa_config(0, &config);
    }

    return false;
}

}  // namespace legacymm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
