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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLORBALANCE_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLORBALANCE_H

#include <vendor/lineage/livedisplay/2.0/IColorBalance.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace legacymm {

using ::android::hardware::Return;
using ::android::hardware::Void;

class ColorBalance : public IColorBalance {
   public:
    ColorBalance(void* libHandle);

    bool isSupported();

    // Methods from ::vendor::lineage::livedisplay::V2_0::IColorBalance follow.
    Return<void> getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) override;
    Return<int32_t> getColorBalance() override;
    Return<bool> setColorBalance(int32_t value) override;

   private:
    void* mLibHandle;

    int (*disp_api_supported)(int32_t, int32_t);
    int (*disp_api_get_color_balance_range)(int32_t, void*);
    int (*disp_api_get_color_balance)(int32_t, int*);
    int (*disp_api_set_color_balance)(int32_t, int);
    int (*disp_api_get_num_display_modes)(int32_t, int32_t, int*);
};

}  // namespace legacymm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLORBALANCE_H
