/*
** Copyright 2016, The CyanogenMod Project
**           2017, The LineageOS Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <cutils/properties.h>

#define LOG_NDEBUG 0

#define LOG_TAG "LiveDisplay-SDM"
#include <utils/Log.h>

#include "SDM.h"
#include "Utils.h"

namespace android {

using hardware::hidl_vec;
using ::vendor::lineage::livedisplay::V1_0::disp_range;
using ::vendor::lineage::livedisplay::V1_0::disp_mode;
using ::vendor::lineage::livedisplay::V1_0::disp_pa_config;
using ::vendor::lineage::livedisplay::V1_0::disp_pa_range;
using ::vendor::lineage::livedisplay::V1_0::version_info;

template<typename T>
using Return = hardware::Return<T>;

status_t SDM::loadHALService() {
    if (intf != nullptr) {
        return OK;
    }

    intf = ::vendor::lineage::livedisplay::V1_0::ILiveDisplay::getService();
    if (intf == nullptr) {
        ALOGE("Failed to load Lineage LiveDisplay HAL service");
        return NO_INIT;
    }
    ALOGI("Loaded Lineage LiveDisplay HAL service");

    return OK;
}

status_t SDM::initialize() {
    status_t rc = loadHALService();
    if (rc != OK) {
        return rc;
    }

    Return<void> ret = intf->init(0,
            [&](int32_t _rc, int64_t hctx) {
                rc = _rc;
                mHandle = hctx;
            });

    if (!ret.isOk()) {
        ALOGE("init failed status: %s", ret.description().c_str());
        return NO_INIT;
    }

    if (rc != OK) {
        return rc;
    }

    mActiveModeId = -1;

    if (hasFeature(Feature::DISPLAY_MODES)) {
        rc = saveInitialDisplayMode();
        if (rc != OK) {
            ALOGE("Failed to save initial display mode! err=%d", rc);
            return rc;
        }
        sp<DisplayMode> defMode = getDefaultDisplayMode();
        if (defMode != nullptr) {
            setDisplayMode(defMode->id, false);
        }
    }
    return OK;
}

status_t SDM::deinitialize() {
    if (intf != nullptr) {
        intf->deinit(mHandle, 0);
        mHandle = -1;
    }
    return OK;
}

status_t SDM::setAdaptiveBacklightEnabled(bool enabled) {
    status_t rc = NO_INIT;
    if (enabled == mCachedFOSSStatus) {
        return OK;
    }
    char* buf = new char[DPPS_BUF_SIZE];
    sprintf(buf, "%s", enabled ? FOSS_ON : FOSS_OFF);
    if (Utils::sendDPPSCommand(buf, DPPS_BUF_SIZE) == OK) {
        if (strncmp(buf, "Success", 7) == 0) {
            rc = OK;
            mCachedFOSSStatus = enabled;
        }
    }
    delete[] buf;
    return rc;
}

bool SDM::isAdaptiveBacklightEnabled() {
    return mCachedFOSSStatus;
}

status_t SDM::getColorBalanceRange(Range& range) {
    status_t rc = OK;
    intf->getGlobalColorBalanceRange(mHandle, 0,
            [&](int32_t _rc, disp_range _range) {
                rc = _rc;
                range.min = _range.min_val;
                range.max = _range.max_val;
                range.step = _range.step_val;
            });
    ALOGV("getColorBalanceRange: min=%d max=%d step=%d", range.min, range.max, range.step);
    return rc;
}

status_t SDM::setColorBalance(int32_t balance) {
    return intf->setGlobalColorBalance(mHandle, 0, balance, 0);
}

int32_t SDM::getColorBalance() {
    int32_t warmness = 0;
    intf->getGlobalColorBalance(mHandle, 0,
            [&](int32_t rc, int32_t _warmness, uint32_t _flags) {
                if (rc != OK) {
                    warmness = 0;
                } else {
                    warmness = _warmness;
                }
            });
    return warmness;
}

uint32_t SDM::getNumDisplayModes() {
    uint32_t flags = 0;
    int32_t mode_cnt = 0;
    intf->getNumDisplayModes(mHandle, 0, 0,
            [&](int32_t rc, int32_t _mode_cnt, uint32_t _flags) {
                if (rc == OK) {
                    mode_cnt = _mode_cnt;
                    flags = _flags;
                }
            });
    if (getLocalSRGBMode() != nullptr) {
        mode_cnt++;
    }
    if (getLocalDCIP3Mode() != nullptr) {
        mode_cnt++;
    }
    return mode_cnt;
}

status_t SDM::getDisplayModes(List<sp<DisplayMode>>& profiles) {
    status_t rc = OK;

    uint32_t sdm_count = getNumDisplayModes();
    if (!sdm_count) return rc;

    sp<DisplayMode> srgb = getLocalSRGBMode();
    sp<DisplayMode> dci_p3 = getLocalDCIP3Mode();
    if (srgb != nullptr) {
        sdm_count--;
    }
    if (dci_p3 != nullptr) {
        sdm_count--;
    }

    intf->getDisplayModes(mHandle, 0, 0, sdm_count,
            [&](int32_t _rc, hidl_vec<disp_mode> modes, uint32_t flags) {
                rc = _rc;
                if (_rc == OK) {
                    for (int i = 0; i < sdm_count; i++) {
                        const sp<DisplayMode> m = new DisplayMode(modes[i].id,
                                modes[i].name.c_str(), modes[i].name_len);
                        m->privFlags = PRIV_MODE_FLAG_SDM;
                        profiles.push_back(m);
                    }
                }
            });

    if (srgb != nullptr) {
        profiles.push_back(srgb);
    }
    if (dci_p3 != nullptr) {
        profiles.push_back(dci_p3);
    }

    return rc;
}

status_t SDM::setDisplayMode(int32_t modeID, bool makeDefault) {
    status_t rc = OK;

    if (modeID == mActiveModeId) {
        return OK;
    }

    sp<DisplayMode> mode = getDisplayModeById(modeID);
    if (mode == nullptr) {
        return BAD_VALUE;
    }

    ALOGV("setDisplayMode: current mode=%d", mActiveModeId);

    if (mActiveModeId >= 0) {
        sp<DisplayMode> oldMode = getCurrentDisplayMode();
        ALOGV("setDisplayMode: oldMode=%d flags=%d", oldMode->id, oldMode->privFlags);
        if (oldMode->privFlags == PRIV_MODE_FLAG_SYSFS ||
                mode->privFlags == PRIV_MODE_FLAG_SYSFS) {
            ALOGV("disabling old mode");
            rc = setModeState(oldMode, false);
            if (rc != OK) {
                ALOGE("Failed to disable previous mode! err=%d", rc);
                return rc;
            }
        }
    }

    rc = setModeState(mode, true);
    if (rc == OK) {
        mActiveModeId = mode->id;
        if (makeDefault) {
            rc = Utils::writeLocalModeId(mode->id);
            if (rc != OK) {
                ALOGE("failed to save mode! %d", rc);
                return rc;
            }
            if (mode->privFlags == PRIV_MODE_FLAG_SDM) {
                rc = intf->setDefaultDisplayMode(mHandle, 0, mode->id, 0);
                if (rc != OK) {
                    ALOGE("failed to save mode! %d", rc);
                    return rc;
                }
            }
        }
        HSIC tmp;
        rc = getPictureAdjustment(tmp);
        if (rc != OK) {
            ALOGE("failed to retrieve picture adjustment after mode setting!");
        } else {
            ALOGV("new default PA: %d %f %f %f %f", tmp.hue, tmp.saturation,
                    tmp.intensity, tmp.contrast, tmp.saturationThreshold);
            mDefaultPictureAdjustment.setTo(tmp);
        }
    } else {
        ALOGE("Failed to setModeState! err=%d", rc);
        return rc;
    }

    ALOGV("setDisplayMode: %d default: %d flags: %d", modeID, makeDefault, mode->privFlags);
    return OK;
}

sp<DisplayMode> SDM::getDisplayModeById(int32_t id) {
    List<sp<DisplayMode>> profiles;
    status_t rc = getDisplayModes(profiles);
    if (rc == OK) {
        for (List<sp<DisplayMode>>::iterator it = profiles.begin(); it != profiles.end(); ++it) {
            const sp<DisplayMode> mode = *it;
            if (id == mode->id) {
                return mode;
            }
        }
    }

    return nullptr;
}

sp<DisplayMode> SDM::getCurrentDisplayMode() {
    return getDisplayModeById(mActiveModeId);
}

sp<DisplayMode> SDM::getDefaultDisplayMode() {
    int32_t id = 0;
    if (Utils::readLocalModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }
    if (Utils::readInitialModeId(&id) == OK && id >= 0) {
        return getDisplayModeById(id);
    }
    return nullptr;
}

status_t SDM::setModeState(sp<DisplayMode> mode, bool state) {
    int32_t id = 0;

    if (mode->privFlags == PRIV_MODE_FLAG_SYSFS) {
        ALOGV("sysfs node: %s state=%d", mode->privData.string(), state);
        return Utils::writeInt(mode->privData.string(), state ? 1 : 0);
    } else if (mode->privFlags == PRIV_MODE_FLAG_SDM) {
        if (state) {
            return intf->setActiveDisplayMode(mHandle, 0, mode->id, 0);
        } else {
            if (Utils::readInitialModeId(&id) == OK) {
                ALOGV("set sdm mode to default: id=%d", id);
                return intf->setActiveDisplayMode(mHandle, 0, id, 0);
            }
        }
    }
    return BAD_VALUE;
}

sp<DisplayMode> SDM::getLocalSRGBMode() {
    char path[PATH_MAX];
    sprintf(path, "%s", SRGB_NODE);

    if (access(path, W_OK) != 0) {
        return nullptr;
    }
    sp<DisplayMode> m = new DisplayMode(SRGB_NODE_ID, "srgb", 4);
    m->privFlags = PRIV_MODE_FLAG_SYSFS;
    m->privData.setTo(path);
    return m;
}

sp<DisplayMode> SDM::getLocalDCIP3Mode() {
    char path[PATH_MAX];
    sprintf(path, "%s", DCI_P3_NODE);

    if (access(path, W_OK) != 0) {
        return nullptr;
    }
    sp<DisplayMode> m = new DisplayMode(DCI_P3_NODE_ID, "dci_p3", 6);
    m->privFlags = PRIV_MODE_FLAG_SYSFS;
    m->privData.setTo(path);
    return m;
}

status_t SDM::getPictureAdjustmentRanges(HSICRanges& ranges) {
    status_t rc;
    intf->getGlobalPARange(mHandle, 0,
            [&](int32_t _rc, disp_pa_range _range) {
                rc = _rc;
                if (rc == OK) {
                    ranges.hue.max = _range.hue.max_val;
                    ranges.hue.min = _range.hue.min_val;
                    ranges.hue.step = _range.hue.step_val;
                    ranges.saturation.max = _range.saturation.max_val;
                    ranges.saturation.min = _range.saturation.min_val;
                    ranges.saturation.step = _range.saturation.step_val;
                    ranges.intensity.max = _range.value.max_val;
                    ranges.intensity.min = _range.value.min_val;
                    ranges.intensity.step = _range.value.step_val;
                    ranges.contrast.max = _range.contrast.max_val;
                    ranges.contrast.min = _range.contrast.min_val;
                    ranges.contrast.step = _range.contrast.step_val;
                    ranges.saturationThreshold.max = _range.sat_threshold.max_val;
                    ranges.saturationThreshold.min = _range.sat_threshold.min_val;
                    ranges.saturationThreshold.step = _range.sat_threshold.step_val;
                }
            });
    return rc;
}

status_t SDM::getPictureAdjustment(HSIC& hsic) {
    status_t rc;
    intf->getGlobalPAConfig(mHandle, 0,
            [&](int32_t _rc, uint32_t enable, disp_pa_config cfg) {
                rc = _rc;
                if (rc == OK) {
                    hsic.hue = cfg.data.hue;
                    hsic.saturation = cfg.data.saturation;
                    hsic.intensity = cfg.data.value;
                    hsic.contrast = cfg.data.contrast;
                    hsic.saturationThreshold = cfg.data.sat_thresh;
                }
            });
    return rc;
}

status_t SDM::getDefaultPictureAdjustment(HSIC& hsic) {
    hsic.setTo(mDefaultPictureAdjustment);
    return OK;
}

status_t SDM::setPictureAdjustment(HSIC hsic) {
    disp_pa_config cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.data.hue = hsic.hue;
    cfg.data.saturation = hsic.saturation;
    cfg.data.value = hsic.intensity;
    cfg.data.contrast = hsic.contrast;
    cfg.data.sat_thresh = hsic.saturationThreshold;

    return intf->setGlobalPAConfig(mHandle, 0, 1, cfg);
}

bool SDM::hasFeature(Feature feature) {
    uint32_t feature_id;
    status_t rc;
    version_info ver;

    switch (feature) {
        case Feature::DISPLAY_MODES:
            feature_id = 4;
            break;
        case Feature::COLOR_TEMPERATURE:
            feature_id = 3;
            break;
        case Feature::PICTURE_ADJUSTMENT:
            feature_id = 1;
        case Feature::ADAPTIVE_BACKLIGHT:
            if (property_get_int32("ro.qualcomm.foss", 0) > 0) {
                return true;
            }
            break;
        default:
            return false;
    }

    intf->getFeatureVersion(mHandle, feature_id,
            [&](int32_t _rc, version_info _ver, uint32_t flags) {
                rc = _rc;
                if (rc == OK) {
                    ver.major = _ver.major;
                    ver.minor = _ver.minor;
                    ver.step = _ver.step;
                }
            });

    if (rc == OK) {
        if (ver.major > 0 || ver.minor > 0 || ver.step > 0) {

            // Color balance depends on calibration data in SDM
            if (feature == Feature::DISPLAY_MODES ||
                    feature == Feature::COLOR_TEMPERATURE) {
                if (getNumDisplayModes() > 0) {
                    // make sure the range isn't zero
                    if (feature == Feature::COLOR_TEMPERATURE) {
                        Range r;
                        if (getColorBalanceRange(r) == OK && r.isNonZero()) {
                            return true;
                        }
                        return false;
                    }
                    return true;
				}
            } else if (feature == Feature::PICTURE_ADJUSTMENT) {
                HSICRanges r;
                if (getPictureAdjustmentRanges(r) == OK && r.isValid()) {
                    return true;
                }
            }
        }
    }
    return false;
}

status_t SDM::saveInitialDisplayMode() {
    int32_t mode_id = 0;
    uint32_t flags = 0;
    status_t rc;
    if (Utils::readInitialModeId(&mode_id) != OK || mode_id < 0) {
        intf->getDefaultDisplayMode(mHandle, 0,
                [&](int32_t _rc, int32_t _mode_id, uint32_t flags) {
                    rc = _rc;
                    if (rc == OK) {
                        mode_id = _mode_id;
                    }
                });
        if (rc == OK && mode_id >= 0) {
            return Utils::writeInitialModeId(mode_id);
        } else {
            return Utils::writeInitialModeId(mode_id = 0);
        }
    }
    return OK;
}

};
