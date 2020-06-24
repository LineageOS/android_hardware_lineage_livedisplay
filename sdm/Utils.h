/*
 * Copyright (C) 2016 The CyanogenMod Project
 *               2017-2020 The LineageOS Project
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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_SDM_UTILS_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_SDM_UTILS_H

#include <stdlib.h>

#include "SDMController.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {
namespace utils {

int SendDPPSCommand(char* buf, size_t len);
status_t CheckFeatureVersion(const std::shared_ptr<SDMController>& controller, int feature);

}  // namespace utils
}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_SDM_UTILS_H
