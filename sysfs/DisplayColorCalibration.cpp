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

#include <android-base/file.h>
#include <android-base/strings.h>

#include <fstream>

#include "DisplayColorCalibration.h"

using android::base::ReadFileToString;
using android::base::Split;
using android::base::Trim;
using android::base::WriteStringToFile;

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sysfs {

bool DisplayColorCalibration::isSupported() {
    std::fstream rgb(FILE_RGB, rgb.in | rgb.out);

    return rgb.good();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayColorCalibration follow.
Return<int32_t> DisplayColorCalibration::getMaxValue() {
    return 32768;
}

Return<int32_t> DisplayColorCalibration::getMinValue() {
    return 255;
}

Return<void> DisplayColorCalibration::getCalibration(getCalibration_cb _hidl_cb) {
    std::vector<int32_t> rgb;
    std::string tmp;

    if (ReadFileToString(FILE_RGB, &tmp)) {
        for (std::string color& : Split(Trim(tmp), " ")) {
            rgb.push_back(std::atoi(color.c_str()));
        }
    }

    _hidl_cb(rgb);
    return Void();
}

Return<bool> DisplayColorCalibration::setCalibration(const hidl_vec<int32_t>& rgb) {
    std::string tmp, contents;

    for (int32_t color : rgb) {
        tmp << rgb << " ";
    }

    contents << Trim(tmp) << std::endl;

    if (WriteStringToFile(contents, FILE_RGB, true)) {
        return true;
    }

    return false;
}

}  // namespace sysfs
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
