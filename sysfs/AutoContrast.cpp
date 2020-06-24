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

#include "AutoContrast.h"

#include <android-base/file.h>
#include <android-base/strings.h>

#include <fstream>

namespace {
constexpr const char* kFileAco = "/sys/class/graphics/fb0/aco";
}  // anonymous namespace

using android::base::ReadFileToString;
using android::base::Trim;
using android::base::WriteStringToFile;

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sysfs {

bool AutoContrast::isSupported() {
    std::fstream aco(kFileAco, aco.in | aco.out);

    return aco.good();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IAutoContrast follow.
Return<bool> AutoContrast::isEnabled() {
    std::string tmp;
    int32_t contents = 0;

    if (ReadFileToString(kFileAco, &tmp)) {
        contents = std::stoi(Trim(tmp));
    }

    return contents > 0;
}

Return<bool> AutoContrast::setEnabled(bool enabled) {
    return WriteStringToFile(enabled ? "1" : "0", kFileAco, true);
}

}  // namespace sysfs
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
