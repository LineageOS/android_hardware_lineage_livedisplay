/*
 * Copyright (C) 2019 The LineageOS Project
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

#include "ColorBalance.h"
#include "Constants.h"
#include "Types.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

ColorBalance::ColorBalance(void* libHandle, uint64_t cookie) {
    mLibHandle = libHandle;
    mCookie = cookie;
    disp_api_get_feature_version =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, void*, uint32_t*)>(
            dlsym(mLibHandle, "disp_api_get_feature_version"));
    disp_api_get_global_color_balance_range =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, void*)>(
            dlsym(mLibHandle, "disp_api_get_global_color_balance_range"));
    disp_api_get_global_color_balance =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, int32_t*, uint32_t*)>(
            dlsym(mLibHandle, "disp_api_get_global_color_balance"));
    disp_api_set_global_color_balance =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, int32_t, uint32_t)>(
            dlsym(mLibHandle, "disp_api_set_global_color_balance"));
    disp_api_get_num_display_modes =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, int32_t, int32_t*, uint32_t*)>(
            dlsym(mLibHandle, "disp_api_get_num_display_modes"));
}

bool ColorBalance::isSupported() {
#if 0
    Range range{};
    sdm_feature_version version{};
    // int32_t count = 0;
    uint32_t flags = 0;

    if (disp_api_get_feature_version == nullptr ||
        disp_api_get_feature_version(mCookie, COLOR_BALANCE_FEATURE, &version, &flags) != 0) {
        return false;
    }

    if (version.x <= 0 && version.y <= 0 && version.z <= 0) {
        return false;
    }

    if (disp_api_get_global_color_balance_range == nullptr ||
        disp_api_get_global_color_balance_range(mCookie, 0, &range) != 0) {
        return false;
    }

    if (range.max == 0 || range.min == 0) {
        return false;
    }

    // This is how this is supposed to work, but it doesn't work quite right
    if (disp_api_get_feature_version(mCookie, DISPLAY_MODES_FEATURE, &version, &flags) == 0 &&
        (version.x > 0 && version.y > 0 && version.z > 0) &&
        disp_api_get_num_display_modes != nullptr &&
        disp_api_get_num_display_modes(mCookie, 0, 0, &count, &flags) == 0) {
        return count > 0;
    }
#endif
    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IColorBalance follow.
Return<void> ColorBalance::getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) {
    Range range{};

    if (disp_api_get_global_color_balance_range != nullptr) {
        if (disp_api_get_global_color_balance_range(mCookie, 0, &range) != 0) {
            range.max = range.min = 0;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<int32_t> ColorBalance::getColorBalance() {
    int32_t value = 0;
    uint32_t flags = 0;

    if (disp_api_get_global_color_balance != nullptr) {
        if (disp_api_get_global_color_balance(mCookie, 0, &value, &flags) != 0) {
            value = 0;
        }
    }

    return value;
}

Return<bool> ColorBalance::setColorBalance(int32_t value) {
    if (disp_api_set_global_color_balance != nullptr) {
        return disp_api_set_global_color_balance(mCookie, 0, value, 0) == 0;
    }

    return false;
}

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
