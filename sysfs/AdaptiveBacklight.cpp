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

#include "AdaptiveBacklight.h"

using android::base::ReadFileToString;
using android::base::Trim;
using android::base::WriteStringToFile;

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sysfs {

bool AdaptiveBacklight::isSupported() {
    std::fstream acl(FILE_ACL, acl.in | acl.out);
    std::fstream cabc(FILE_CABC, cabc.in | cabc.out);

    if (acl.good()) {
        mFile = FILE_ACL;
    } else if (cabc.good()) {
        mFile = FILE_CABC;
    }

    return !mFile.empty();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> AdaptiveBacklight::isEnabled() {
    std::string tmp;
    int32_t contents;

    if (ReadFileToString(mFile, &tmp)) {
        contents = std::stoi(Trim(tmp));
        return contents > 0;
    }

    return false;
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    return WriteStringToFile(enabled ? "1" : "0", mFile, true);
}

}  // namespace sysfs
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
