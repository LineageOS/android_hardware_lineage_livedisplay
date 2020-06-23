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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYMODES_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYMODES_H

#include <android-base/macros.h>
#include <vendor/lineage/livedisplay/2.0/IDisplayModes.h>

#include "SDMController.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

using ::android::hardware::hidl_string;
using ::android::hardware::Return;
using ::android::hardware::Void;

class DisplayModes : public IDisplayModes {
  public:
    explicit DisplayModes(std::shared_ptr<SDMController> controller);
    static bool isSupported(const hidl_string& name = "default");

    using DisplayModeSetCallback = std::function<void()>;
    void registerDisplayModeSetCallback(DisplayModeSetCallback callback);

    // Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
    Return<void> getDisplayModes(getDisplayModes_cb _hidl_cb) override;
    Return<void> getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) override;
    Return<void> getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) override;
    Return<bool> setDisplayMode(int32_t modeID, bool makeDefault) override;

  private:
    std::shared_ptr<SDMController> controller_;
    DisplayModeSetCallback on_display_mode_set_;

    bool isReady();
    std::vector<DisplayMode> getDisplayModesInternal();
    DisplayMode getDisplayModeById(int32_t id);
    DisplayMode getCurrentDisplayModeInternal();
    DisplayMode getDefaultDisplayModeInternal();

    DISALLOW_IMPLICIT_CONSTRUCTORS(DisplayModes);
};

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYMODES_H
