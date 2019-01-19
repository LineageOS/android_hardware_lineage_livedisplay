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
namespace legacymm {

ColorBalance::ColorBalance(void* libHandle) {
    mLibHandle = libHandle;
    disp_api_supported =
        reinterpret_cast<int (*)(int32_t, int32_t)>(dlsym(mLibHandle, "disp_api_supported"));
    disp_api_get_color_balance_range = reinterpret_cast<int (*)(int32_t, void*)>(
        dlsym(mLibHandle, "disp_api_get_color_balance_range"));
    disp_api_get_color_balance =
        reinterpret_cast<int (*)(int32_t, int*)>(dlsym(mLibHandle, "disp_api_get_color_balance"));
    disp_api_set_color_balance =
        reinterpret_cast<int (*)(int32_t, int)>(dlsym(mLibHandle, "disp_api_set_color_balance"));
    disp_api_get_num_display_modes = reinterpret_cast<int (*)(int32_t, int32_t, int*)>(
        dlsym(mLibHandle, "disp_api_get_num_display_modes"));
}

bool ColorBalance::isSupported() {
    mm_cb_range range{};
    int count = 0;

    if (disp_api_supported == nullptr || disp_api_supported(0, COLOR_BALANCE_FEATURE) == 0) {
        return false;
    }

    if (disp_api_get_color_balance_range == nullptr ||
        disp_api_get_color_balance_range(0, &range) != 0) {
        return false;
    }

    if (range.max == 0 || range.min == 0) {
        return false;
    }

    // This is how this is supposed to work, but it doesn't work quite right
    /*
        if (disp_api_supported(0, DISPLAY_MODES_FEATURE) != 0 &&
            disp_api_get_num_display_modes != nullptr &&
            disp_api_get_num_display_modes(0, 0, &count) == 0) {
            return count > 0;
        }
    */

    return false;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IColorBalance follow.
Return<void> ColorBalance::getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) {
    Range range{};
    mm_cb_range r{};

    if (disp_api_get_color_balance_range != nullptr) {
        if (disp_api_get_color_balance_range(0, &range) == 0) {
            range.min = r.min;
            range.max = r.max;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<int32_t> ColorBalance::getColorBalance() {
    int value = 0;

    if (disp_api_get_color_balance != nullptr) {
        if (disp_api_get_color_balance(0, &value) != 0) {
            value = 0;
        }
    }

    return static_cast<int32_t>(value);
}

Return<bool> ColorBalance::setColorBalance(int32_t value) {
    if (disp_api_set_color_balance != nullptr) {
        return disp_api_set_color_balance(0, static_cast<int>(value)) == 0;
    }

    return false;
}

}  // namespace legacymm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
