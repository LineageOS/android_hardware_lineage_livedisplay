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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_SDM_TYPES_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_SDM_TYPES_H

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

struct SdmFeatureVersion {
    uint8_t x, y;
    uint16_t z;
};

struct SdmDispMode {
    int32_t id;
    int32_t type;
    int32_t len;
    char* name;
    SdmDispMode() : id(-1), type(0), len(128) { name = new char[128](); }
    ~SdmDispMode() { delete[] name; }
};

struct HsicData {
    int32_t hue;
    float saturation;
    float intensity;
    float contrast;
    float saturation_threshold;
};

struct HsicConfig {
    uint32_t unused;
    HsicData data;
};

struct HsicIntRange {
    int32_t max;
    int32_t min;
    uint32_t step;
};

struct HsicFloatRange {
    float max;
    float min;
    float step;
};

struct HsicRanges {
    uint32_t unused;
    struct HsicIntRange hue;
    struct HsicFloatRange saturation;
    struct HsicFloatRange intensity;
    struct HsicFloatRange contrast;
    struct HsicFloatRange saturation_threshold;
};

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_SDM_TYPES_H
