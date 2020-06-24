/*
 * Copyright (C) 2018-2020 The LineageOS Project
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

#pragma once

#include <android-base/macros.h>
#include <utils/Errors.h>

#include <functional>
#include <memory>

#include "Types.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

using ::android::status_t;

class SDMController {
  public:
    SDMController();
    ~SDMController();

    status_t getNumDisplayModes(int32_t* mode_cnt);
    status_t getDisplayModes(SdmDispMode* modes, int32_t mode_cnt);
    status_t getActiveDisplayMode(int32_t* mode_id);
    status_t setActiveDisplayMode(int32_t mode_id);
    status_t setDefaultDisplayMode(int32_t mode_id);
    status_t getDefaultDisplayMode(int32_t* mode_id);
    status_t getGlobalPaRange(HsicRanges* range);
    status_t getGlobalPaConfig(HsicConfig* cfg);
    status_t setGlobalPaConfig(HsicConfig* cfg);
    status_t getFeatureVersion(uint32_t feature_id, SdmFeatureVersion* ver);

  private:
    status_t init();
    status_t deinit();

    typedef int32_t (*disp_api_init)(uint64_t*, uint32_t);
    typedef int32_t (*disp_api_deinit)(uint64_t, uint32_t);
    typedef int32_t (*disp_api_get_num_display_modes)(uint64_t, uint32_t, int32_t, int32_t*,
                                                      uint32_t*);
    typedef int32_t (*disp_api_get_display_modes)(uint64_t, uint32_t, int32_t, SdmDispMode*,
                                                  int32_t, uint32_t*);
    typedef int32_t (*disp_api_get_active_display_mode)(uint64_t, uint32_t, int32_t*, uint32_t*,
                                                        uint32_t*);
    typedef int32_t (*disp_api_set_active_display_mode)(uint64_t, uint32_t, int32_t, uint32_t);
    typedef int32_t (*disp_api_set_default_display_mode)(uint64_t, uint32_t, int32_t, uint32_t);
    typedef int32_t (*disp_api_get_default_display_mode)(uint64_t, uint32_t, int32_t*, uint32_t*);
    typedef int32_t (*disp_api_get_global_pa_range)(uint64_t, uint32_t, HsicRanges*);
    typedef int32_t (*disp_api_get_global_pa_config)(uint64_t, uint32_t, uint32_t*, HsicConfig*);
    typedef int32_t (*disp_api_set_global_pa_config)(uint64_t, uint32_t, uint32_t, HsicConfig*);
    typedef int32_t (*disp_api_get_feature_version)(uint64_t, uint32_t, SdmFeatureVersion*,
                                                    uint32_t*);

    std::unique_ptr<void, std::function<void(void*)>> handle_;
    uint64_t hctx_;

    disp_api_init fn_init_;
    disp_api_deinit fn_deinit_;
    disp_api_get_num_display_modes fn_get_num_display_modes_;
    disp_api_get_display_modes fn_get_display_modes_;
    disp_api_get_active_display_mode fn_get_active_display_mode_;
    disp_api_set_active_display_mode fn_set_active_display_mode_;
    disp_api_set_default_display_mode fn_set_default_display_mode_;
    disp_api_get_default_display_mode fn_get_default_display_mode_;
    disp_api_get_global_pa_range fn_get_global_pa_range_;
    disp_api_get_global_pa_config fn_get_global_pa_config_;
    disp_api_set_global_pa_config fn_set_global_pa_config_;
    disp_api_get_feature_version fn_get_feature_version_;

    DISALLOW_COPY_AND_ASSIGN(SDMController);
};

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
