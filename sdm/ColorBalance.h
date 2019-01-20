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
namespace sdm {

using ::android::hardware::Return;
using ::android::hardware::Void;

class ColorBalance : public IColorBalance {
   public:
    ColorBalance(void* libHandle, uint64_t cookie);

    bool isSupported();

    // Methods from ::vendor::lineage::livedisplay::V2_0::IColorBalance follow.
    Return<void> getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) override;
    Return<int32_t> getColorBalance() override;
    Return<bool> setColorBalance(int32_t value) override;

   private:
    void* mLibHandle;
    uint64_t mCookie;

    int32_t (*disp_api_get_feature_version)(uint64_t, uint32_t, void*, uint32_t*);
    int32_t (*disp_api_get_global_color_balance_range)(uint64_t, uint32_t, void*);
    int32_t (*disp_api_get_global_color_balance)(uint64_t, uint32_t, int32_t*, uint32_t*);
    int32_t (*disp_api_set_global_color_balance)(uint64_t, uint32_t, int32_t, uint32_t);
    int32_t (*disp_api_get_num_display_modes)(uint64_t, uint32_t, int32_t, int32_t*, uint32_t*);
};

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_COLORBALANCE_H
