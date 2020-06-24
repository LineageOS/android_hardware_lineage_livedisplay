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

#include "AdaptiveBacklight.h"

#include <android-base/properties.h>
#include <cutils/sockets.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>

namespace {
constexpr size_t kDppsBufSize = 64;

constexpr const char* kFossProperty = "ro.vendor.display.foss";
constexpr const char* kFossOn = "foss:on";
constexpr const char* kFossOff = "foss:off";

int SendDPPSCommand(char* buf, size_t len) {
    int rc = 0;
    int sock = socket_local_client("pps", ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
    if (sock < 0) {
        return sock;
    }

    if (write(sock, buf, strlen(buf) + 1) > 0) {
        memset(buf, 0, len);
        ssize_t ret;
        while ((ret = read(sock, buf, len)) > 0) {
            if ((size_t)ret == len) {
                break;
            }
            len -= ret;
            buf += ret;

            struct pollfd p = {.fd = sock, .events = POLLIN, .revents = 0};

            ret = poll(&p, 1, 20);
            if ((ret <= 0) || !(p.revents & POLLIN)) {
                break;
            }
        }
    } else {
        rc = -EIO;
    }

    close(sock);
    return rc;
}
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

using ::android::base::GetBoolProperty;

bool AdaptiveBacklight::isSupported() {
    return GetBoolProperty(kFossProperty, false);
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> AdaptiveBacklight::isEnabled() {
    return enabled_;
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    if (enabled_ == enabled) {
        return true;
    }

    auto buf = std::make_unique<char[]>(kDppsBufSize);

    sprintf(buf.get(), "%s", enabled ? kFossOn : kFossOff);
    if (SendDPPSCommand(buf.get(), kDppsBufSize) == 0) {
        if (strncmp(buf.get(), "Success", 7) == 0) {
            enabled_ = enabled;
            return true;
        }
    }

    return false;
}

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
